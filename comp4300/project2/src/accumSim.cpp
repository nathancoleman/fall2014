/*
*	Accumulator Simulator
*         
* 	By Lucas Saltz & Nathan Coleman
*	Group 15
*	COMP4300 - Computer Architecture
*	
*	Project 1 - Due 12 September 2014
*	Project 2 - Due 03 October 2014
*/

#include "accumSim.h"

int main(int argc, char* argv[]) {
	
	if (argc > 1)
	{
		FILENAME = argv[1];
	}
	else
	{
		printf("ERROR: Expected filename\n");
		exit(1);
	}
	
	init();
	
	execute();
	
	//printDebug(0,5);

	return 0;
}



/*
*	This function handles execution of the instructions
*	for the accumulator simulation.
*/
int execute()
{	
	printf("Executing...\n");
	
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 26;
		//mem_addr address = instr & ((1 << 24) - 1);
		printf("%s\n", "Inside the while loop");
		// Encode the instruction
		//instr = ADDI;
		//instr = instr << 26; // first 6 bits are op code
		//instr |= dest << 21; // second 5 bits are dest
		//instr |= src << 16; // third 5 bits are src
		//instr |= imm; // last 16 bits are immediate

		//printf("\t\t\tOp Code: %x\n", instr >> 26);
		//printf("\t\t\tDest: %x\n", instr >> 21);
		//printf("\t\t\tSrc: %x\n", (instr >> 16) & 0x001F);
		//printf("\t\t\tImm: %x\n", instr & 0x0000FFFF);


		if (op == ADDI) 
		{
			/*
				Pulling out registers from instruction
			*/
			int dest =  instr >> 21; 
			int src = instr >> 16;
			int imm = instr & 0x0000FFFF;

			R[dest] = R[src] + imm; //Registers array 

			printf("%s\n", "addi function implemented");
			
		}
		
		else if (op == B) 
		{}
		
		else if (op == BEQZ)
		{}
		
		else if (op == BGE)
		{}
		
		else if (op == BNE)
		{}

		else if (op == LA)
		{}

		else if (op == LB)
		{}

		else if (op == LI)
		{}

		else if (op == SUBI)
		{}

		else if (op == SYSCALL)
		{}
		
		else
		{
			throw runtime_error("*** MEMORY ERROR *** : Invalid instruction reached");
		}
		
		PC++;
	}
	
	printf("Execution complete!\n");
	return 0;
}

