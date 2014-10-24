#include "util.h"

bool is_branch_instr(instruction instr)
{
	int32 op_code = instr >> 26; // Op code is first 6 bits

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