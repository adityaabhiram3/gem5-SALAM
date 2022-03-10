#include "../search_clstr_hw_defines.h"

typedef struct BTree{
   int d[7];
   struct BTree * child_ptr[8];
   int l;
   int n;
} bTree;


//SEQUENTIAL

void top(unsigned * a, unsigned* b, unsigned *c) {

	bTree * cursor = (bTree* )*c;
	unsigned key = *a;
	while(!cursor->l){
		for(int i = 0; i < cursor->n; i++){
			if(key<cursor->d[i])
			{
				cursor = cursor->child_ptr[i];
				
				break;
			}
			if(key == cursor->d[i])
			{
				*b=1;
				cursor = cursor->child_ptr[i];
				break;
			}
			if(i == cursor->n - 1){
				cursor = cursor->child_ptr[i+1];
			}
		}

	}
	for(int i = 0; i < cursor->n; i++){
		if(key == cursor->d[i])
		{
			*b=1;
			break;
		}
	}

	return;

  
}



