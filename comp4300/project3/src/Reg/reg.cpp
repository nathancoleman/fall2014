#include "reg.h"

/* Initialize externs */
reg_word R[R_LENGTH];

void init_regs()
{
	
}

int32 get(int index)
{
	return R[index];
}

void set(int index, int32 val)
{
	R[index] = val;
}