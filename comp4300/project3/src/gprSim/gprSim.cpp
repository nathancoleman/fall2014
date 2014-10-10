#include "gprSim.h"

/* Initialize externs */
std::string FILENAME;

int main(int argc, char* argv[])
{
	if (argc > 1)
		FILENAME = argv[1];
	else
		throw std::runtime_error("*** RUNTIME ERROR *** : Filename expected");

	init();

	execute();

	return 0;
}

void init()
{
	init_regs();

	init_segs();
}

void execute()
{
	printf("PC: %x\n", PC);
}

void writeResult(int C, int IC) 
{
	float speedUpRatio = ((8*(float)IC) / (float)C);
	FILE *fp;
	fp = fopen("result.txt", "w");

	fprintf(fp, "The total IC count: %d\n", IC);
	fprintf(fp, "The C count: %d\n", C);
	fprintf(fp, "The speedup:  %f\n", speedUpRatio);
}