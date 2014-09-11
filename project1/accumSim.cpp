/*
Accumulator Simulator
         
By Lucas Saltz
Group 15 with Nathan Coleman
COMP4300 - Computer Architecture
Project 1 - Due 12 September 2014
*/

#include "accumSim.h"


/*
____________________THINGS TO DO___________________________
1. Edit ADD to add value with accumulator address value
2. Edit MULT to mult value with accumulator
3. Add STORE - store accumulator value into listed address
5. Add LOAD - load an address value into accumulator

*/


string segment;
string line;
fstream myfile;
string FILENAME;


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
		

	return 0;
}

/*
*	This function handles execution of the instructions
*	in TEXT_SEG after it has been initialized. Execution
*	begins at the address PC is pointing to, TEXT_SEG_BASE
*	and continues until it reaches TEXT_TOP, the address
*	the final instruction.
*/
int execute()
{	
	printf("Executing...\n");
	
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 24;
		mem_addr address = instr & ((1 << 24) - 1);
		
		if (op == LOAD) //was PUSH
		{
			printf("LOAD [%x]\n", address);
			
			write(TOS, read(address));
		}
		
		else if (op == STORE) //was POP
		{
			printf("STORE [%x]\n", address);
			
			mem_word val1 = read(--TOS);

			/*
			*	Reset the data at this position since
			*	we are taking one from the top of the
			*	stack and putting nothing in its place
			*/
			STACK_SEG[TOS - STACK_SEG_BASE] = 0;
			
			write(address, val1);	
		}
		
		else if (op == ADD)
		{
			printf("ADD\n");
			
			mem_word val1 = read(--TOS);

			
			//*	Reset the data at this position since
			//*	we are taking two from the stack and
			//*	only returning the result
			
			STACK_SEG[TOS - STACK_SEG_BASE] = 0;
			mem_word val2 = read(--TOS);
						
			write(TOS, val2 + val1);	
		}
		
		
		else if (op == MULT)
		{
			printf("MULT\n");
	
			mem_word val1 = read(--TOS);

			/*
			*	Reset the data at this position since
			*	we are taking two from the stack and
			*	only returning the result
			*/
			STACK_SEG[TOS - STACK_SEG_BASE] = 0;
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 * val1);
		}
		
		
		else if (op == END)
		{
			printf("END\n");
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

