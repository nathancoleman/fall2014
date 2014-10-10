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
}

void execute()
{
	printf("PC: %x\n", PC);
}