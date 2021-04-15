#ifndef MACROS_HH
#define MACROS_HH
//------------------------------------------//
#include <iomanip>
#include <iostream>

// Function Macros
#define MIN(a,b) (((a)<(b))?(a):(b))

// Data type sizing based off LLVM references
#define SYSTEMSIZE          64  // Bit size of system
#define BYTE                8   // Size of a byte in bits
#define VOIDSIZE            0   // Void data type size
#define BYTESIZE(X)         (((X-1)/8)+1)       // Convert bits to bytes
#define DEFAULTSIZE         (SYSTEMSIZE/BYTE)  // Default register size
#define POINTERSIZE         (SYSTEMSIZE/BYTE)   // Pointer data type size
#define LABELSIZE           (SYSTEMSIZE/BYTE)   // Label data type size
#define DOUBLESIZE          (SYSTEMSIZE/BYTE)   // Double data type size
#define FLOATSIZE           (SYSTEMSIZE/(2*BYTE)) // Float data type size

// Indexing
#define SKIPFIRST           1   // Start string on second character
#define COMPAREFOUND        0   // Index 0 returned if entire string is found

// Hardware Units
#define    COUNTER          0
#define    INTADDER         1
#define    INTMULTI         2
#define    INTSHIFTER       3
#define    INTBITWISE       4
#define    FPSPADDER        5
#define    FPDPADDER        6
#define    FPSPMULTI        7
#define    FPSPDIVID        8
#define    FPDPMULTI        9
#define    FPDPDIVID        10
#define    COMPARE          11
#define    GETELEMENTPTR    12
#define    CONVERSION       13
#define    OTHER            14
#define    REGISTER         15

// Hardware Unit Staging
#define    COUNTER_STAGES          1
#define    INTADDER_STAGES         1
#define    INTMULTI_STAGES         1
#define    INTSHIFTER_STAGES       1
#define    INTBITWISE_STAGES       1
#define    FPSPADDER_STAGES        3
#define    FPDPADDER_STAGES        3
#define    FPSPMULTI_STAGES        3
#define    FPSPDIVID_STAGES        3
#define    FPDPMULTI_STAGES        3
#define    FPDPDIVID_STAGES        3
#define    COMPARE_STAGES          1
#define    GETELEMENTPTR_STAGES    1
#define    CONVERSION_STAGES       1
#define    REGISTER_STAGES         1
#define    OTHER_STAGES            1

//Flags
#define ZEROEXT 			0x00000000001
#define SIGNEXT				0x00000000002
#define INREG				0x00000000004
#define BYVAL				0x00000000008
#define SRET				0x00000000010
#define NOALIAS				0x00000000020
#define NOCAPTURE			0x00000000040
#define NEST				0x00000000080
#define RETURNED			0x00000000100
#define ALIGNSTACK			0x00000000200
#define ALWAYSINLINE		0x00000000400
#define BUILTIN				0x00000000800
#define COLD				0x00000001000
#define INLINEINT			0x00000002000
#define MINSIZE				0x00000004000
#define NAKED				0x00000008000
#define NOBUILTIN			0x00000010000
#define NODUPLICATE			0x00000020000
#define NOIMPLICITFLOAT		0x00000040000
#define NOINLINE			0x00000080000
#define NONLAZYBIND			0x00000100000
#define NOREDZONE			0x00000200000
#define NORETURN			0x00000400000
#define NOUNWIND			0x00000800000
#define OPTNONE				0x00001000000
#define OPTSIZE				0x00002000000
#define READNONE			0x00004000000
#define READONLY			0x00008000000
#define RETURNS_TWICE		0x00010000000
#define SANITIZE_MEMORY		0x00020000000
#define SANITIZE_ADDRESS	0x00040000000
#define SANITIZE_THREAD		0x00080000000
#define SSP					0x00100000000
#define SSPREQ				0x00200000000
#define SSPSTRONG			0x00400000000
#define UWTABLE				0x00800000000
#define CCC					0x01000000000
#define FASTCC				0x02000000000
#define COLDCC				0x04000000000
#define CC10				0x08000000000
#define CC11				0x10000000000
#define VOLATILE			0x20000000000
#define INBOUNDS			0x40000000000

#define NNAN				0x00000000001
#define NINF				0x00000000002
#define NSZ					0x00000000004
#define ARCP				0x00000000008
#define CONTRACT			0x00000000010
#define AFN					0x00000000020
#define REASSOC				0x00000000040
#define FAST				0x00000000080
#define NSW					0x00000000100
#define NUW					0x00000000200
#define EXACT				0x00000000400
#define EQ					0x00000000800
#define NE					0x00000001000
#define UGT					0x00000002000
#define UGE					0x00000004000
#define ULT					0x00000008000
#define ULE					0x00000010000
#define SGT					0x00000020000
#define SGE					0x00000040000
#define SLT					0x00000080000
#define SLE					0x00000100000
#define CONDFALSE			0x00000200000
#define CONDTRUE			0x00000400000
#define OEQ					0x00000800000
#define OGT					0x00001000000
#define OGE					0x00002000000
#define OLT					0x00004000000
#define OLE					0x00008000000
#define ONE					0x00010000000
#define ORD					0x00020000000
#define UEQ					0x00040000000
#define UNE					0x00080000000
#define UNO					0x00100000000

// LLVM Instructions Definitions (Re-Define From llvm/IR/Instructions.def)
#define LLVM_IR_Move 0
#define LLVM_IR_Ret 1
#define LLVM_IR_Br 2
#define LLVM_IR_Switch 3
#define LLVM_IR_IndirectBr 4
#define LLVM_IR_Invoke 5
#define LLVM_IR_Resume 6
#define LLVM_IR_Unreachable 7
#define LLVM_IR_Add 13
#define LLVM_IR_FAdd 14
#define LLVM_IR_Sub 15
#define LLVM_IR_FSub 16
#define LLVM_IR_Mul 17
#define LLVM_IR_FMul 18
#define LLVM_IR_UDiv 19
#define LLVM_IR_SDiv 20
#define LLVM_IR_FDiv 21
#define LLVM_IR_URem 22
#define LLVM_IR_SRem 23
#define LLVM_IR_FRem 24
#define LLVM_IR_Shl 25
#define LLVM_IR_LShr 26
#define LLVM_IR_AShr 27
#define LLVM_IR_And 28
#define LLVM_IR_Or 29
#define LLVM_IR_Xor 30
#define LLVM_IR_Alloca 31
#define LLVM_IR_Load 32
#define LLVM_IR_Store 33
#define LLVM_IR_GetElementPtr 34
#define LLVM_IR_Fence 35
#define LLVM_IR_AtomicCmpXchg 36
#define LLVM_IR_AtomicRMW 37
#define LLVM_IR_Trunc 38
#define LLVM_IR_ZExt 39
#define LLVM_IR_SExt 40
#define LLVM_IR_FPToUI 41
#define LLVM_IR_FPToSI 42
#define LLVM_IR_UIToFP 43
#define LLVM_IR_SIToFP 44
#define LLVM_IR_FPTrunc 45
#define LLVM_IR_FPExt 46
#define LLVM_IR_PtrToInt 47
#define LLVM_IR_IntToPtr 48
#define LLVM_IR_BitCast 49
#define LLVM_IR_AddrSpaceCast 50
#define LLVM_IR_ICmp 53
#define LLVM_IR_FCmp 54
#define LLVM_IR_PHI 55
#define LLVM_IR_Call 56
#define LLVM_IR_Select 57
#define LLVM_IR_VAArg 60
#define LLVM_IR_ExtractElement 61
#define LLVM_IR_InsertElement 62
#define LLVM_IR_ShuffleVector 63
#define LLVM_IR_ExtractValue 64
#define LLVM_IR_InsertValue 65
#define LLVM_IR_LandingPad 66
#define LLVM_IR_DMAFence 97
#define LLVM_IR_DMAStore 98
#define LLVM_IR_DMALoad 99
#define LLVM_IR_IndexAdd 100
#define LLVM_IR_SilentStore 101
#define LLVM_IR_Sine 102
#define LLVM_IR_Cosine 103


namespace SALAM {
    enum Predicate : unsigned {
        FCMP_FALSE = 0, FCMP_OEQ = 1, FCMP_OGT = 2, FCMP_OGE = 3,
        FCMP_OLT = 4, FCMP_OLE = 5, FCMP_ONE = 6, FCMP_ORD = 7,
        FCMP_UNO = 8, FCMP_UEQ = 9, FCMP_UGT = 10, FCMP_UGE = 11,
        FCMP_ULT = 12, FCMP_ULE = 13, FCMP_UNE = 14, FCMP_TRUE = 15,
        FIRST_FCMP_PREDICATE = FCMP_FALSE, LAST_FCMP_PREDICATE = FCMP_TRUE, BAD_FCMP_PREDICATE = FCMP_TRUE + 1, ICMP_EQ = 32,
        ICMP_NE = 33, ICMP_UGT = 34, ICMP_UGE = 35, ICMP_ULT = 36,
        ICMP_ULE = 37, ICMP_SGT = 38, ICMP_SGE = 39, ICMP_SLT = 40,
        ICMP_SLE = 41, FIRST_ICMP_PREDICATE = ICMP_EQ, LAST_ICMP_PREDICATE = ICMP_SLE, BAD_ICMP_PREDICATE = ICMP_SLE + 1
    };
}


#endif //__MACROS_HH__
