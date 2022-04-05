#include "../search_clstr_hw_defines.h"
#include <stdint.h>


//SEQUENTIAL

void top(uint64_t a, uint64_t b, uint64_t c) {

	volatile uint8_t *search = (uint8_t*)(SEARCH+0x00);
	volatile uint32_t *s_arg1 = (uint32_t*)(SEARCH+0x01);
	volatile uint32_t *s_arg2 = (uint32_t*)(SEARCH+0x09);
	volatile uint32_t *s_arg3 = (uint32_t*)(SEARCH+0X11);
	volatile uint32_t *s_arg4 = (uint32_t*)(SEARCH+0X19);

	volatile uint8_t *vector1 = (uint8_t*)(VECTOR1+0x00);
	volatile uint32_t *v_arg1 = (uint32_t*)(VECTOR1+0x01);
	volatile uint32_t *v_arg2 = (uint32_t*)(VECTOR1+0x09);
	volatile uint32_t *v_arg3 = (uint32_t*)(VECTOR1+0X11);

	volatile uint8_t *vector2 = (uint8_t*)(VECTOR2+0x00);
	volatile uint32_t *v2_arg1 = (uint32_t*)(VECTOR2+0x01);
	volatile uint32_t *v2_arg2 = (uint32_t*)(VECTOR2+0x09);
	volatile uint32_t *v2_arg3 = (uint32_t*)(VECTOR2+0X11);

	volatile uint8_t * root  = (uint8_t *)ROOTCACHE;

	uint64_t m1;
	uint64_t m2;
	uint64_t m3;
	uint64_t m4;
	m1=(uint64_t)(void*)0x80c00000;
	m2=(uint64_t)(void*)0x80c00fa0;
	m3=(uint64_t)(void*)0x80c01f40;
	m4=(uint64_t)(void*)0x80c02f40;
	unsigned* t = (unsigned*)m4;
	*t = 0;
	int i = 0;
	int jl = 0;
	int count = 0;

	// while(count < 5){
		// *s_arg1 = m1;
		// *s_arg2 =m2;
		// *s_arg3 =m3;

		
		// *search = 0x1;
		// *search = 0x01;
		// while((*search & 4) != 4)
		// {
		// 	count++;
		// }
		// *search = 0x0;

		// count++;

		// *s_arg1 = a + 40;
		// *s_arg2 =b;
		// *s_arg3 =c;

		
		// *search = 0x1;
		// while((*search & 4) != 4)
		// {
		// 	count++;
		// }
		// *search = 0x0;

		// count++;


	// }

	

	// b[0] = a[5];

	while(jl <= 1900){

		*s_arg1 = m1 + jl;
		*s_arg2 = m2 + jl;
		*s_arg3 = m3;
		while(*s_arg1 != m1 + jl && *s_arg2 != m2 + jl && *s_arg3 != m3);

		*v_arg1 =m1 + jl + 4;
		*v_arg2 =m2 + jl + 4;
		*v_arg3 =m3;

		while(*v_arg1 != m1 + jl && *v_arg2 != m2 + jl && *v_arg3 != m3);

		*v2_arg1 =(m1 + jl + 8);
		*v2_arg2 =(m2 + jl + 8);
		*v2_arg3 =m3;

		while(*v2_arg1 != m1 + jl && *v2_arg2 != m2 + jl && *v2_arg3 != m3);

		jl +=12;

		*search = 0x1;
		// *search = 0x01;
		// // *root = 0x1;
		while((*search & 4) != 4)
		{
			count++;
		}
		// // *root = 0x0;
		*search = 0x0;

		*vector1 = 0x1;
		while((*vector1 & 4) != 4)
		{
			count++;
		}
		*vector1 = 0x0;

		*vector2 = 0x1;
		while((*vector2 & 4) != 4)
		{
			count++;
		}
		*vector2=0x0;


	}

	

	return;

  
}





