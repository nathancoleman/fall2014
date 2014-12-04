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

	print_instr_status();
}

void print_instr_status()
{
	int arraySize = TEXT_TOP - TEXT_SEG_BASE;

	int i;
	for (i = 0; i < arraySize; i++)
		printf("%d %d %d %d\n", instr_status[i][ISSUE], instr_status[i][READ_OPS], instr_status[i][EXEC_COMP], instr_status[i][WRITE_RES]);
}
