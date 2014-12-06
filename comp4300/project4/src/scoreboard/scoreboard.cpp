#include "scoreboard.h"

string FILENAME;
unsigned int cycleCount = 1;

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
}

void init_res_status()
{
	for (int i = 0; i < R_LENGTH + F_LENGTH; i++)
		res_status[i] = UNUSED;
}

void run()
{
	printf("Executing... PC = %x\n", PC);

	bool user_mode = true;

	while (user_mode)	
	{
		instruction instr = read_mem(PC);
		
		printf("\tInstruction: %x\n", getBits(instr, 26, 32));

		if (can_issue_instr(instr))
			issue_instr(instr);

		cycleCount++;

		if (PC == TEXT_TOP)
			user_mode = false;

		if (!user_mode)
			printf("\tHALTING EXECUTION\n");

		else
			PC++;

	}

	print_instr_status_board();
	print_res_status_board();
}

bool can_issue_instr(instruction instr)
{
	bool can_issue = true;

	if (check_for_waw(instr))
	{
		printf("\t\t################ FOUND WAW ##################\n");
		can_issue = false;
	}

	if (check_for_str_haz(instr))
	{
		printf("\t\t################ FOUND STR HAZ ##############\n");
		can_issue = false;
	}

	return can_issue;
}

bool check_for_waw(instruction instr)
{
	int dest = getBits(instr, 16, 21);

	for (int i = 0; i < 5; i++)
		if (fu_status[i][FI] == dest)
			return true;

	return false;
}

bool check_for_str_haz(instruction instr)
{
	int dest = getBits(instr, 16, 21);

	if (res_status[dest] != UNUSED)
	{
		printf("Dest %d is being used by FU %d\n", dest, res_status[dest]);
		return true;
	}

	return false;
}

void issue_instr(instruction instr)
{
	printf("\t\tIssuing...\n");
	instr_status[PC-TEXT_SEG_BASE][ISSUE] = cycleCount;

	int FU = UNUSED;

	int32 op_code = getBits(instr, 26, 32);
	int32 dest = getBits(instr, 16, 21);

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
			FU = ADD_FU;
			break;
	}

	res_status[dest] = FU;
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
	printf("\nBusy\tOp\tFi\tFj\tFk\tQj\tQk\tRj\tRk\n");
	printf("--------------------------------------------------------------------\n");

	int i;
	for (i = 0; i < arraySize; i++)
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", fu_status[i][0], fu_status[i][1], fu_status[i][2], fu_status[i][3], fu_status[i][4],
			fu_status[i][5], fu_status[i][6], fu_status[i][7], fu_status[i][8]);
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
