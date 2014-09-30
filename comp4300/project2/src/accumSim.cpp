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

mem_addr TEMP = 0x00300004;
mem_addr ACCUMULATOR = 0x00300005;



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
*	for the accumulator simulation. Values at given addresses 
*	are either multiplied, added, loaded or stored with the 
*	ACCUMULATOR.
*/
int execute()
{	
	printf("Executing...\n");
	
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 24;
		mem_addr address = instr & ((1 << 24) - 1);
		
		if (op == LOAD) 
		{
			printf("LOAD [%x]\n", address);
			
			write(ACCUMULATOR, read(address));
		}
		
		else if (op == STORE) 
		{
			printf("STORE [%x]\n", address);
			
			mem_word val1 = read(ACCUMULATOR);

			write(address, val1);	
		}
		
		else if (op == ADD)
		{
			printf("ADD\n");
			
			mem_word val1 = read(address);
			mem_word val2 = read(ACCUMULATOR);
						
			write(ACCUMULATOR, val2 + val1);	
		}
		
		
		else if (op == MULT)
		{
			printf("MULT\n");
	
			mem_word val1 = read(address);
			mem_word val2 = read(ACCUMULATOR);
			
			write(ACCUMULATOR, val2 * val1);
		}
		
		
		else if (op == END)
		{
			printf("END\n");
			printf("ANSWER: %i\n", read(ACCUMULATOR));
			break;	
		}
		
		else
		{
			throw runtime_error("*** MEMORY ERROR *** : Invalid instruction reached");
		}
		
		PC++;
	}
	
	printf("Execution complete!\n");
	return 0;
}

