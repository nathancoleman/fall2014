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
}

void run()
{
	printf("Executing...\n");

	if (can_issue_instr())
		issue_instr();
}

bool can_issue_instr()
{
	return true;
}

void issue_instr()
{
	instruction next_instr = read_mem(PC);
	instr_status[PC-TEXT_SEG_BASE][ISSUE] = cycleCount;

	print_instr_status_board();
	print_fu_status_board();
	print_res_status_board();
}

void print_instr_status_board()
{
	int arraySize = TEXT_TOP - TEXT_SEG_BASE;
	printf("\nIssue\tReOps\tExComp\tWrRes\n");
	printf("------------------------------\n");

	int i;
	for (i = 0; i < arraySize; i++)
		printf("%d\t%d\t%d\t%d\n", instr_status[i][ISSUE], instr_status[i][READ_OPS], instr_status[i][EXEC_COMP], instr_status[i][WRITE_RES]);
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
	int arraySize = F_LENGTH;
	printf("\n");

	int i;
	for (i = 0; i < arraySize; i++)
		printf("F%d\t", i);

	printf("\n");

}
