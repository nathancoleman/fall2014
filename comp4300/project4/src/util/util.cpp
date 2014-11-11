#include "util.h"

int32 getBits(instruction instr, int32 min, int32 max)
{
	int32 mask = ~(~0 << (max - min + 1));
	int32 result = instr;
	result = result >> min;
	result = result & mask;

	return result;
}

bool is_branch_instr(int32 op_code)
{
	switch (op_code)
	{
		case B:
			return true;
			break;

		case BEQZ:
			return true;
			break;

		case BGE:
			return true;
			break;

		case BNE:
			return true;
			break;
	}

	return false;
}