#include "reg.h"

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