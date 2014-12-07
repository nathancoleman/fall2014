#include "scoreboard.h"

string FILENAME;
unsigned int cycleCount = 1;
unsigned int lastComplete = 0;

int main(int argc, char* argv[])
{
	if (argc > 1)
		FILENAME = argv[1];
	else
		throw std::runtime_error("*** RUNTIME ERROR *** : Filename expected");

	init();

	run();

	return 0;
}

void init()
{
	init_regs();

	init_segs(FILENAME);

	init_instr_table();

	init_res_status();

	init_fu_status();
}

void init_res_status()
{
	for (int i = 0; i < R_LENGTH + F_LENGTH; i++)
		res_status[i] = UNUSED;
}

void init_fu_status()
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 9; j++)
			fu_status[i][j] = UNUSED;
}

void run()
{
	printf("Executing... PC = %x\n", PC);

	bool user_mode = true;

	while (user_mode)
	{
		instruction instr = read_mem(PC);
		bool issued_instr = false;

		printf("PC is %x\n", PC);
		printf("\tInstruction: %x\n", getBits(instr, 26, 31));

		// if (can_issue_instr(instr))
		// {
			issue_instr(instr);
			issued_instr = true;
		// }

		maintain_instr();

		cycleCount++;

		if (PC == TEXT_TOP)
			user_mode = false;

		if (!user_mode)
			printf("HALTING EXECUTION!\n");

		// PC++ if we haven't ended execution and an instruction was executed
		if (user_mode && issued_instr)
			PC++;

	}

	print_instr_status_board();
	print_fu_status_board();
	print_res_status_board();
}

bool can_issue_instr(instruction instr)
{
	bool can_issue = true;

	int32 FU = get_fu(instr);
	if (fu_status[FU][BUSY] == 1)
	{
		printf("\t\t################## FU BUSY ####################\n");
		can_issue = false;
	}

	if (check_for_waw(instr))
	{
		printf("\t\t################## FOUND WAW ##################\n");
		can_issue = false;
	}

	if (check_for_str_haz(instr))
	{
		printf("\t\t################ FOUND STR HAZ ################\n");
		can_issue = false;
	}

	return can_issue;
}

bool check_for_waw(instruction instr)
{
	int dest = getBits(instr, 16, 20);

	for (int i = 0; i < 5; i++)
		if (fu_status[i][FI] == dest)
			return true;

	return false;
}

bool check_for_str_haz(instruction instr)
{
	int dest = getBits(instr, 16, 20);

	if (res_status[dest] != UNUSED)
	{
		printf("Dest %d is being used by FU %d\n", dest, res_status[dest]);
		return true;
	}

	return false;
}

int32 get_fu(instruction instr)
{
	int32 op_code = getBits(instr, 26, 31);
	int32 FU = UNUSED;

	switch (op_code)
	{
		case B:
		case BEQZ:
		case BGE:
		case BNE:
		case LI:
		case LD:
			FU = INTEGER_FU;
			break;

		case FMUL:
			if (!fu_status[MULT1_FU][BUSY])
				FU = MULT1_FU;
			else if (!fu_status[MULT2_FU][BUSY])
				FU = MULT2_FU;
			break;

		case ADD:
		case ADDI:
		case SUBI:
		case FADD:
		case FSUB:
			//by lucas

			if (!fu_status[ADD_FU][BUSY])
				FU = ADD_FU;
			else
				//wait
			break;
	}

	return FU;
}

void issue_instr(instruction instr)
{
	printf("\t\tIssuing...\n");
	instr_status[PC-TEXT_SEG_BASE][ISSUE] = cycleCount;

	int FU = get_fu(instr);

	int32 op_code = getBits(instr, 26, 31);
	int32 dest = (instr >> 16) & 0b11111;//getBits(instr, 16, 20);
	int32 src = getBits(instr, 21, 25);
	int32 tar = getBits(instr, 11, 15);
	printf("\t\tDEST is %d\n", dest);

	// Unused in case of NOP
	if (FU != UNUSED)
	{
		fu_status[FU][BUSY] = 1;
		fu_status[FU][OP] = op_code;
		fu_status[FU][FI] = dest;
		fu_status[FU][FJ] = src;
		fu_status[FU][FK] = tar;
	}

	res_status[dest] = FU;
}

void maintain_instr()
{
	if (!all_complete())
	{
		for (int i = 0; i < 5; i++)
		{
			bool get_ops_complete = true;
			if (fu_status[i][FJ] != UNUSED && can_get_op(fu_status[i][FJ]))
			{
				int32 reg = fu_status[i][FJ];
				printf("RETRIEVING VAL FROM REG %d\n", reg);
			}
			else
				get_ops_complete = false;

			if (fu_status[i][FK] != UNUSED && can_get_op(fu_status[i][FK]))
			{
				int32 reg = fu_status[i][FK];
				printf("RETRIEVING VAL FROM REG %d\n", reg);
			}
			else
				get_ops_complete = false;

			if (get_ops_complete)
			{
				fu_status[i][RJ] = 1;
				fu_status[i][RK] = 1;
			}

			// instr_status[PC-TEXT_SEG_BASE][READ_OPS] = cycleCount + 1;
			// instr_status[PC-TEXT_SEG_BASE][EXEC_COMP] = cycleCount + 2;
			// instr_status[PC-TEXT_SEG_BASE][WRITE_RES] = cycleCount + 3;
		}					
	}
}

bool all_complete()
{
	return lastComplete == TEXT_TOP - TEXT_SEG_BASE;
}

bool can_get_op(int32 reg_index)
{
	return true;
}

void get_ops()
{

}

bool check_for_raw(int32 reg_index)
{
	if (res_status[reg_index] != UNUSED)
	{
		printf("Register is being used by FU %d\n", res_status[reg_index]);
		return true;
	}

	return false;
}

void print_instr_status_board()
{
	int arraySize = TEXT_TOP - TEXT_SEG_BASE;
	printf("\nIssue\tReOps\tExComp\tWrRes\n");
	printf("------------------------------\n");

	int i;
	for (i = 0; i < arraySize; i++)
	{
		if (instr_status[i][ISSUE] != 0)
			printf("%d\t", instr_status[i][ISSUE]);
		else
			printf(" \t");

		if (instr_status[i][READ_OPS] != 0)
			printf("%d\t", instr_status[i][READ_OPS]);
		else
			printf(" \t");

		if (instr_status[i][EXEC_COMP] != 0)
			printf("%d\t", instr_status[i][EXEC_COMP]);
		else
			printf(" \t");

		if (instr_status[i][WRITE_RES] != 0)
			printf("%d\n", instr_status[i][WRITE_RES]);
		else
			printf(" \n");
	}
}

void print_fu_status_board()
{
	int arraySize = 5;
	printf("\n\tBusy\tOp\tFi\tFj\tFk\tQj\tQk\tRj\tRk\n");
	printf("\t--------------------------------------------------------------------\n");

	int i;
	for (i = 0; i < arraySize; i++)
	{
		switch (i)
		{
			case 0:
				printf("INT\t");
				break;
			case 1:
				printf("MULT1\t");
				break;
			case 2:
				printf("MULT2\t");
				break;
			case 3:
				printf("ADD\t");
				break;
			case 4:
				printf("DIV\t");
				break;
		}

		if (fu_status[i][0] != UNUSED)
			printf("%d\t", fu_status[i][BUSY]);
		else
			printf(" \t");

		if (fu_status[i][1] != UNUSED)
			printf("%x\t", fu_status[i][OP]);
		else
			printf(" \t");

		if (fu_status[i][2] != UNUSED)
			printf("%d\t", fu_status[i][FI]);
		else
			printf(" \t");

		if (fu_status[i][3] != UNUSED)
			printf("%d\t", fu_status[i][FJ]);
		else
			printf(" \t");

		if (fu_status[i][4] != UNUSED)
			printf("%d\t", fu_status[i][FK]);
		else
			printf(" \t");

		if (fu_status[i][5] != UNUSED)
			printf("%d\t", fu_status[i][QJ]);
		else
			printf(" \t");

		if (fu_status[i][6] != UNUSED)
			printf("%d\t", fu_status[i][QK]);
		else
			printf(" \t");

		if (fu_status[i][7] != UNUSED)
			printf("%d\t", fu_status[i][RJ]);
		else
			printf(" \t");

		if (fu_status[i][8] != UNUSED)
			printf("%d\n", fu_status[i][RK]);
		else
			printf(" \t\n");
	}
}

void print_res_status_board()
{
	int arraySize = R_LENGTH + F_LENGTH;
	printf("\n");

	int i;
	for (i = 0; i < arraySize/2; i++)
		printf("R%d\t", i);

	printf("\n");

	for (i = 0; i < arraySize/2; i++)
		if (res_status[i] == UNUSED)
			printf(" \t");
		else
			printf("%d\t", res_status[i]);

	printf("\n\n");

	for (i = R_LENGTH; i < arraySize; i++)
		printf("F%d\t", i - R_LENGTH);

	printf("\n");

	for (i = R_LENGTH; i < arraySize; i++)
		if (res_status[i] == UNUSED)
			printf(" \t");
		else
			printf("%d\t", res_status[i]);

	printf("\n");
}
