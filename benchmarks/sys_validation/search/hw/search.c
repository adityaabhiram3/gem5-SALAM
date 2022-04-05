#include "../search_clstr_hw_defines.h"
#include <stdint.h>

typedef struct BTree{
   int d[7];
   struct BTree * child_ptr[8];
   int l;
   int n;
} bTree;

volatile uint8_t * root  = (uint8_t *)ROOTCACHE;
typedef struct rootCacheNode{
	int start;
	int end;
	int key;
	int utility;

} node_t;

typedef struct rootCache{
	node_t entry[10];
	int occupancy;
} cache_t;


typedef struct rc_query{
	unsigned start;
	unsigned end;
	unsigned start_address;
	unsigned flag;
} query_t;

//SEQUENTIAL

void request(query_t query, query_t* stream_in){

	*stream_in = query;
	return;

}

int response(unsigned* stream_out, unsigned * status){
	int temp = 20;
	if(*status)
		temp = *stream_out; 
	return temp;

}


void top(unsigned * a, unsigned* b, unsigned *c) {


	query_t* strOut = (query_t *)cache_search_in;
	
	unsigned key = *a;

	query_t q;

	q.start = key;
	q.end = key;
	q.start_address = key;
	q.flag = key;
	*strOut = q;
	// request(q, strOut);

	unsigned* strIn = (unsigned* )cache_search_out;
	unsigned* status = (unsigned* )0x100207f0;

	unsigned temp = 100;
	
  	// b[0] = temp;
	// while(*status == 0);
	// while(*flag == temp)
		// temp = response(strIn, status);
	// *flag = temp;
	*b = response(strIn, status);

	// bTree* cursor = (bTree*)*c;
	// uint64_t key = *a;
	
	// while(!cursor->l){
	// 	for(int i = 0; i < cursor->n; i++){
	// 		if(key<cursor->d[i])
	// 		{
	// 			cursor = cursor->child_ptr[i];
				
	// 			break;
	// 		}
	// 		if(key == cursor->d[i])
	// 		{
	// 			*b=1;
	// 			cursor = cursor->child_ptr[i];
	// 			break;
	// 		}
	// 		if(i == cursor->n - 1){
	// 			cursor = cursor->child_ptr[i+1];
	// 			break;
	// 		}
	// 	}
	// }

	// for(int i = 0; i < cursor->n; i++){
		
	// 	if(key == cursor->d[i])
	// 	{
	// 		*b=1;
	// 		break;
	// 	}
	// }





	return;

  
}

// void top(uint64_t key, uint64_t out, uint64_t root) {
	
// 	// unsigned* inStatus = (unsigned *)0x100206f0;
// 	unsigned* status = (unsigned *)0x100207f0;
// 	query_t* strOut = (query_t*)0x10020600;
// 	unsigned* strIn = (unsigned *)cache_search_out;
// 	unsigned * key_search = (unsigned *)0x80c00000;
// 	unsigned * key_rc    = (unsigned *)0x80c00fa0;
//     // unsigned * g    = (unsigned *)0x80c10008;
// 	// unsigned * m    = (unsigned *)0x80c10010;
//     cache_t * h     = (cache_t *)TESTA;
// 	int count;
// 	unsigned temp = 0;
// 	// uint64_t key = *a;

// 	unsigned key_val = *(unsigned *)key_search;
// 	unsigned* result = (unsigned *)key_rc;


// 	temp = key_val;
// 	bTree* cursor;
// 	bTree* ref;
// 	query_t q;
// 	q.start = temp;
// 	q.end = temp;
// 	q.start_address = temp;
// 	q.flag = temp;

// 	*strOut = q;

// 	result[0] = *strIn;
// 	// request(q, strOut);
// 	// temp = response(strIn, status);
// 	// int i = 0;
// 	// while(1){
// 	// 	if(*status != 0){
// 	// 		// temp = *strIn;
// 	// 		*b = *strIn;
// 	// 		// search_stream(strIn, strOut, c);
// 	// 		// *strOut = temp.start;
// 	// 		return;
// 	// 	}
// 	// 	else{
// 	// 		i++;
// 	// 	}

		
// 	// }
// 	// while(1)
// 	// if(*status > 0)
		

// 	// b[0] = 1000;
	

// 	// *strOut = q;
	
// 	// if(!outStatus)
// 	// 	*b = *strIn;

    
	

// 	return;

  
// }