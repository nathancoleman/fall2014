#include "reg.h"

/* Initialize externs */
reg_word R[R_LENGTH];
float_word F[F_LENGTH];

void init_regs()
{
	
}

reg_word read_reg(int index)
{
	if (index >= R_LENGTH)
		throw std::runtime_error("*** REGISTER ERROR *** : Invalid register index");

	return R[index];
}

float_word read_freg(int index)
{
	if (index >= F_LENGTH)
		throw std::runtime_error("*** REGISTER ERROR *** : Invalid register index");

	return F[index];
}

void write_reg(int index, reg_word val)
{
	if (index >= R_LENGTH)
		throw std::runtime_error("*** REGISTER ERROR *** : Invalid register index");

	R[index] = val;
}

void write_freg(int index, float_word val)
{
	if (index >= F_LENGTH)
		throw std::runtime_error("*** REGISTER ERROR *** : Invalid register index");

	F[index] = val;
}