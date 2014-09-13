#include "stackSim.h"

/*
*	This is just the main entry point that begins the
*	initialization routine and then hands off execution
*	to the execute() function. printDebug() is used at
*	the end to visualize the status of segments and pointers
*/
int main(int argc, char* argv[])
{
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
		
		if (op == PUSH)
		{
			printf("PUSH [%x]\n", address);
			
			write(TOS, read(address));
		}
		
		else if (op == POP)
		{
			printf("POP [%x]\n", address);
			
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

			/*
			*	Reset the data at this position since
			*	we are taking two from the stack and
			*	only returning the result
			*/
			write(TOS, 0, false);
			mem_word val2 = read(--TOS);
						
			write(TOS, val2 + val1);	
		}
		
		else if (op == SUB)
		{
			printf("SUB\n");
			
			mem_word val1 = read(--TOS);

			/*
			*	Reset the data at this position since
			*	we are taking two from the stack and
			*	only returning the result
			*/
			write(TOS, 0, false);
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 - val1);	
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
			write(TOS, 0, false);
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 * val1);
		}
		
		else if (op == DIV)
		{
			printf("DIV\n");
						
			mem_word val1 = read(--TOS);

			/*
			*	Reset the data at this position since
			*	we are taking two from the stack and
			*	only returning the result
			*/
			write(TOS, 0, false);
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 / val1);	
		}
		
		else if (op == END)
		{
			printf("END\n");
			printf("ANSWER: %i\n", read(--TOS));
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