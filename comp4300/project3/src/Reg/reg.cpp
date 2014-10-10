#include "reg.h"

/* Initialize externs */
reg_word R[R_LENGTH];

void init_regs()
{
	
}

int32 get(int index)
{
	if (index >= R_LENGTH)
		throw std::runtime_error("*** MEMORY ERROR *** : Invalid register index");

	return R[index];
}

void set(int index, int32 val)
{
	if (index >= R_LENGTH)
		throw std::runtime_error("*** MEMORY ERROR *** : Invalid register index");
	
	R[index] = val;
}