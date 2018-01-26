#include "hwacc/comm_interface.hh"

#include "base/trace.hh"
#include "debug/CommInterface.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"
#include "sim/system.hh"

#include <stdio.h>
#include <stdlib.h>


using namespace std;

CommInterface::CommInterface(Params *p) :
    BasicPioDevice(p, p->pio_size),
    io_addr(p->pio_addr),
    io_size(p->pio_size),
    devname(p->devicename),
    gic(p->gic),
    int_num(p->int_num),
    memPort(p->name + ".mem_side", this),
    masterId(p->system->getMasterId(name())),
    tickEvent(this),
    cacheLineSize(p->cache_line_size),
    clock_period(p->clock_period) {
    processDelay = 1000 * clock_period;
    needToRead = false;
    needToWrite = false;
    running = false;
    processingDone = false;
    computationNeeded = false;
    dataPort = &memPort;
    mmreg = new uint8_t[io_size];
    cu = NULL;
}

bool
CommInterface::MemSidePort::recvTimingResp(PacketPtr pkt) {
    owner->recvPacket(pkt);
    return true;
}

void
CommInterface::MemSidePort::recvReqRetry() {
    assert(outstandingPkts.size());

    DPRINTF(CommInterface, "Got a retry...\n");
    while (outstandingPkts.size() && sendTimingReq(outstandingPkts.front())) {
        DPRINTF(CommInterface, "Unblocked, sent blocked packet.\n");
        outstandingPkts.pop();
        // TODO: This should just signal the engine that the packet completed
        // engine should schedule tick as necessary. Need a test case
        if (!owner->tickEvent.scheduled()) {
            owner->schedule(owner->tickEvent, curTick() + owner->processDelay);
        }
    }
}

void
CommInterface::MemSidePort::sendPacket(PacketPtr pkt) {
    if (isStalled() || !sendTimingReq(pkt)) {
        DPRINTF(CommInterface, "sendTiming failed in sendPacket(pkt->req->getPaddr()=0x%x)\n", (unsigned int)pkt->req->getPaddr());
        setStalled(pkt);
    }
}

void
CommInterface::recvPacket(PacketPtr pkt) {
    if (pkt->isRead()) {
        DPRINTF(CommInterface, "Done with a read. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        pkt->writeData(curData + (pkt->req->getPaddr() - beginAddr));

        for (int i = pkt->req->getPaddr() - beginAddr;
             i < pkt->req->getPaddr() - beginAddr + pkt->getSize(); i++)
        {
            readsDone[i] = true;
        }

        // mark readDone as only the contiguous region
        while (readDone < totalLength && readsDone[readDone])
        {
            readDone++;
        }

        if (readDone >= totalLength)
        {
            DPRINTF(CommInterface, "done reading!!\n");
            needToRead = false;
            running = false;
            processData();
        }
    } else {
        DPRINTF(CommInterface, "Done with a write. addr: 0x%x, size: %d\n", pkt->req->getPaddr(), pkt->getSize());
        writeDone += pkt->getSize();
        if (!(writeDone < totalLength)) {
            DPRINTF(CommInterface, "Done writing, completely done\n");
            gic->sendInt(int_num);
            *(uint32_t *)mmreg |= 0x80000000;
            DPRINTF(CommInterface, "MMReg value: 0x%016x\n", *(uint64_t *)mmreg);
            needToWrite = false;
            delete[] curData;
            delete[] readsDone;
            running = false;
        } else {
            if (!tickEvent.scheduled())
            {
                schedule(tickEvent, curTick() + processDelay);
            }
        }
    }
    if (pkt->req) delete pkt->req;
    delete pkt;
}

void
CommInterface::tick() {
    if (!running) {
        DPRINTF(CommInterface, "Checking MMR to see if Run bit set\n");
        if (*mmreg & 0x01) {
            *mmreg &= 0xfe;
            computationNeeded = true;
            prepRead(mmrval, 0x04);
        }

        if (processingDone && !tickEvent.scheduled()) {
            processingDone = false;
            schedule(tickEvent, curTick() + processDelay);
        }

        return;
    }
    if (dataPort->isStalled()) {
        DPRINTF(CommInterface, "Stalled\n");
    } else {
        if (needToRead && !dataPort->isStalled()) {
            DPRINTF(CommInterface, "trying read\n");
            tryRead();
        }

        if (needToWrite && !dataPort->isStalled() &&
            ((totalLength - writeLeft) < readDone)) {
            DPRINTF(CommInterface, "trying write\n");
            tryWrite();
        }
    }
}

void
CommInterface::tryRead() {
    //RequestPtr req = new Request();
    Request::Flags flags;

    if (readLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to read if there aren't reads left\n");
        return;
    }

    int size;
    if (currentReadAddr % cacheLineSize) {
        size = cacheLineSize - (currentReadAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = readLeft > (size - 1) ? size : readLeft;
    RequestPtr req = new Request(currentReadAddr, size, flags, masterId);

    DPRINTF(CommInterface, "Trying to read addr: 0x%x, %d bytes\n",
        req->getPaddr(), size);

    PacketPtr pkt = new Packet(req, MemCmd::ReadReq);
    pkt->allocate();
    dataPort->sendPacket(pkt);

    currentReadAddr += size;

    readLeft -= size;

    if (!(readLeft > 0)) {
        needToRead = false;
        if (!tickEvent.scheduled()) {
            schedule(tickEvent, curTick() + processDelay);
        }
    } else {
        if (!dataPort->isStalled() && !tickEvent.scheduled())
        {
            schedule(tickEvent, curTick() + processDelay);
        }
    }
}

void
CommInterface::tryWrite() {
    if (writeLeft <= 0) {
        DPRINTF(CommInterface, "Something went wrong. Shouldn't try to write if there aren't writes left\n");
        return;
    }

    int size;
    if (currentWriteAddr % cacheLineSize) {
        size = cacheLineSize - (currentWriteAddr % cacheLineSize);
        DPRINTF(CommInterface, "Aligning\n");
    } else {
        size = cacheLineSize;
    }
    size = writeLeft > size - 1 ? size : writeLeft;

    Request::Flags flags;
    uint8_t *data = new uint8_t[size];
    std::memcpy(data, &curData[totalLength-writeLeft], size);
    RequestPtr req = new Request(currentWriteAddr, size, flags, masterId);
    req->setExtraData((uint64_t)data);


    DPRINTF(CommInterface, "totalLength: %d, writeLeft: %d\n", totalLength, writeLeft);
    DPRINTF(CommInterface, "Trying to write to addr: 0x%x, %d bytes, data 0x%08x\n",
        currentWriteAddr, size, *((int*)(&curData[totalLength-writeLeft])));

    PacketPtr pkt = new Packet(req, MemCmd::WriteReq);
    uint8_t *pkt_data = (uint8_t *)req->getExtraData();
    pkt->dataDynamic(pkt_data);
    dataPort->sendPacket(pkt);

    currentWriteAddr += size;
    writeLeft -= size;

    if (!(writeLeft > 0) && !tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + processDelay);
    }
}

int
CommInterface::prepRead(Addr src, size_t length) {
    assert(length > 0);
    assert(!running);
    running = true;
    gic->clearInt(int_num);

    DPRINTF(CommInterface, "Initiating read of %d bytes from 0x%x\n", length, src);

    needToRead = true;
    needToWrite = false;

    currentReadAddr = src;

    beginAddr = src;

    readLeft = length;
    writeLeft = 0;

    totalLength = length;

    readDone = 0;

    curData = new uint8_t[length];
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        curData[i] = 0;
        readsDone[i] = false;
    }

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + processDelay);
    }

    return 0;
}

int
CommInterface::prepWrite(Addr dst, uint8_t* value, size_t length) {
    assert(!running);
    assert(length > 0);
    running = true;
    gic->clearInt(int_num);
    *(uint32_t *)mmreg &= 0xefffffff;

    DPRINTF(CommInterface, "Initiating write of %d bytes at 0x%x to 0x%x\n",
        length, dst, value);

    needToRead = false;
    needToWrite = true;

    currentWriteAddr = dst;

    readLeft = 0;
    writeLeft = length;

    totalLength = length;
    writeLeft = totalLength;

    readDone = length;
    writeDone = 0;

    curData = new uint8_t[length];
    readsDone = new bool[length];
    for (int i = 0; i < length; i++) {
        curData[i] = *(value + i);
        readsDone[i] = true;
    }

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + processDelay);
    }

    return 0;
}

Tick
CommInterface::read(PacketPtr pkt) {
    DPRINTF(CommInterface, "The address range associated with this ACC was read!\n");

    Addr daddr = pkt->req->getPaddr() - io_addr;

    uint32_t data;

    switch(daddr) {
        case DEV_CONFIG:
            data = *(uint32_t *)mmreg;
            break;
        default:
            data = *(uint32_t *)(mmreg + 4);
            break;
    }

    switch(pkt->getSize()) {
      case 1:
        pkt->set<uint8_t>(data);
        break;
      case 2:
        pkt->set<uint16_t>(data);
        break;
      case 4:
        pkt->set<uint32_t>(data);
        break;
      default:
        panic("Read size too big?\n");
        break;
    }

    pkt->makeAtomicResponse();
    return pioDelay;
}

Tick
CommInterface::write(PacketPtr pkt) {
    DPRINTF(CommInterface,
        "The address range associated with this ACC was written to!\n");

    pkt->writeData(mmreg + (pkt->req->getPaddr() - io_addr));

    mmrval = *(uint32_t *)(mmreg + 4);

    DPRINTF(CommInterface, "MMReg value: 0x%016x\n", *(uint64_t *)mmreg);

    pkt->makeAtomicResponse();

    if (!tickEvent.scheduled()) {
        schedule(tickEvent, nextCycle());
    }

    return pioDelay;
}

void
CommInterface::processData() {
    DPRINTF(CommInterface, "BEGIN: Processing Data\n");
    uint8_t data[4];
    for (int i = 0; i<4; i++) data[i] = *(curData + i);
    DPRINTF(CommInterface, "Data: 0x%08x\n", data);

    *data = (*(int *)data / 2);
    DPRINTF(CommInterface, "Data: 0x%08x\n", data);
    processingDone = true;

    prepWrite(mmrval, data, 4);
}

CommInterface *
CommInterfaceParams::create() {
    return new CommInterface(this);
}

BaseMasterPort&
CommInterface::getMasterPort(const std::string& if_name, PortID idx) {
    if (if_name == "mem_side") {
        return memPort;
    } else {
        return MemObject::getMasterPort(if_name, idx);
    }
}
