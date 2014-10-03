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


		if (op == ADDI) 
		{
			printf("\tExecuting ADDI\n");
			/*
				Pulling out registers from instruction
			*/
			int dest =  (instr >> 21) & 0x1F; 
			int src = (instr >> 16) & 0x1F;
			int imm = instr & 0xFFFF;

			R[dest] = R[src] + imm; //Registers array 

			printf("\t\tRESULT: %d\n", R[dest]);
			//instr_table_line entry = instr_table[ADDI];
			//TODO
			//increment total from the instr_table_line
		}
		
		else if (op == B) 
		{
			printf("\tExecuting B\n");
		}
		
		else if (op == BEQZ)
		{
			printf("\tExecuting BEQZ\n");

			int src = (instr >> 21) & 0x1F;
			int offset = instr & 0xFFFF;
			int val = R[src];
			if (val == 0)
			{
				printf("\t\tMoving to offset %d\n", offset);
				PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop
			}
		}
		
		else if (op == BGE)
		{
			printf("\tExecuting BGE\n");
			int src1 = instr >> 21;
			int src2 = instr >> 16;

			//TODO: obtain value of label to branch to
		}
		
		else if (op == BNE)
		{
			printf("\tExecuting BNE\n");

			int src1 = instr >> 21;
			int src2 = instr >> 16;
			//TODO: obtain value of label to branch to
		}

		else if (op == LA)
		{
			printf("\tExecuting LA\n");

			int dest = instr >> 21;

			//TODO: obtain value of label(address)
		}

		else if (op == LB)
		{
			printf("\tExecuting LB\n");

			int src = instr >> 21;
			//TODO: Grab offset of second param
		}

		else if (op == LI)
		{
			printf("\tExecuting LI\n");

			int dest = instr >> 21;
			int imm = instr & 0xFFFF; //grabbing last 16bits for imm value???
			//R[dest] = imm;
		}

		else if (op == SUBI)
		{
			printf("\tExecuting SUBI\n");

			int dest = (instr >> 21) & 0x1F;
			int src = (instr >> 16) & 0x1F;

			//////////////////////////////////////
			int imm; 
			imm = instr & 0xFFFF; //grabbing last 16 bits?
			//OR
			//imm = instr << 0x0000FFFF;
			/////////////////////////////////////

			R[dest] = R[src] - imm;
			printf("\t\tRESULT: %d\n", R[dest]);
		}


		/*
		Idea: Syscall looks at the following registers and their values:
		$v0 =	$20
		$a0 =	$10
		$a1 =	$11
		$v0 codes:
			4	-	$a0 = address of string in memory 
			8	-	$a0 = memory address of string input buffer
					$a1 = length of string buffer (n)
			10	-	$a0 = amount , results: address in $v0
		*/
		else if (op == SYSCALL)
		{
			printf("\tExecuting SYSCALL\n");

		//TODO: Comeplete

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

