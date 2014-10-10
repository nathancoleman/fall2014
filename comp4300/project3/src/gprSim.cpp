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

#include "gprSim.h"

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

	int cycles = 0;
	int IC = 0;
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 26;
		// printf("%s\n", "Inside the while loop");


		if (op == ADDI) 
		{
			cycles += instruction_totals[ADDI];
			IC += 1;
			printf("\tExecuting ADDI\n");
			int dest =  (instr >> 21) & 0x1F; 
			int src = (instr >> 16) & 0x1F;
			int imm = instr & 0xFFFF;

			R[dest] = R[src] + imm; //Registers array 

			printf("\t\tRESULT: %x\n", R[dest]);
		}
		
		else if (op == B) 
		{
			cycles += instruction_totals[B];
			IC += 1;
			printf("\tExecuting B\n");

			int offset = instr & 0xFFFF;

			printf("\t\tMoving to offset %d\n", offset);
			PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop
		}
		
		else if (op == BEQZ)
		{
			printf("\tExecuting BEQZ\n");
			cycles += instruction_totals[BEQZ];
			IC += 1;
			int src = (instr >> 21) & 0x1F;
			printf("\t\tSrc is %d\n", src);
			int offset = instr & 0xFFFF;
			int val = R[src] & 0xFF;
			printf("\t\tValue in reg %x is %x\n", src, val);
			if (val == 0)
			{
				printf("\t\tMoving to offset %d\n", offset);
				PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop
			}
		}
		
		else if (op == BGE)
		{
			cycles += instruction_totals[BGE];
			IC += 1;
			printf("\tExecuting BGE\n");
			int src1 = (instr >> 21) & 0x1F;
			int src2 = (instr >> 16) & 0x1F;
			int offset = instr & 0xFFFF;
			int val1 = R[src1];
			int val2 = R[src2];
			if (val1 >= val2)
			{
				printf("\t\tMoving to offset %d\n", offset);
				PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop
			}
		}
		
		else if (op == BNE)
		{
			printf("\tExecuting BNE\n");
			cycles += instruction_totals[BNE];
			IC += 1;
			int src1 = (instr >> 21) & 0x1F;
			int src2 = (instr >> 16) & 0x1F;
			int offset = instr & 0xFFFF;
			int val1 = R[src1];
			int val2 = R[src2];
			if (val1 != val2)
			{
				printf("\t\tMoving to offset %d\n", offset);
				PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop	
			}
		}

		else if (op == LA)
		{
			cycles += instruction_totals[LA];
			IC += 1;
			printf("\tExecuting LA\n");

			int dest = (instr >> 21) & 0x1F;
			int address = instr & 0xFFFF;

			printf("\t\tPlacing address %x in register %d\n", address, dest);

			R[dest] = address;
		}

		else if (op == LB)
		{
			cycles += instruction_totals[LB];
			IC += 1;
			printf("\tExecuting LB\n");

			int dest = (instr >> 21) & 0x1F;
			int src = (instr >> 16) & 0x1F;
			printf("\t\tsrc: %x\tdest: %x\n", src, dest);
			mem_addr address = R[src];
			printf("\t\tAddress: %x\n", address);
			// TODO!
			mem_word result = read(address);
			result = result & 0x000000FF; // First 8 bits is byte at address
			R[dest] = result;
			printf("\t\tResult: %c\n", R[dest]);
		}

		else if (op == LI)
		{
			cycles += instruction_totals[LI];
			IC += 1;
			printf("\tExecuting LI\n");

			int dest = (instr >> 21) & 0x1F;
			int imm = instr & 0xFFFF; //grabbing last 16bits for imm value???
			R[dest] = imm;
		}

		else if (op == SUBI)
		{
			//cycles += 6;
			cycles += instruction_totals[SUBI];
			IC += 1;
			printf("\tExecuting SUBI\n");

			int dest = (instr >> 21) & 0x1F;
			int src = (instr >> 16) & 0x1F;

			int imm; 
			imm = instr & 0xFFFF; //grabbing last 16 bits?
			printf("\t\tORIGINAL: %x\n", R[src]);
			R[dest] = R[src] - imm;
			printf("\t\tRESULT: %x\n", R[dest]);
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
			cycles += instruction_totals[SYSCALL];
			IC += 1;
			printf("\tExecuting SYSCALL\n");

			if (R[29] == 8)
			{
				printf("(input string):>");
				string input;
				getline(cin, input);
				mem_addr address = R[30];
				int size = R[31];

				int i;
				for (i = 0; i < input.length(); i++)
				{
					if (i > size)
					{
						break;
					}
					write(address, input[i], false);
					address++;
				}
				write(address, '\n', false);
				address++;
			}
			else if (R[29] == 4)
			{
				string msg = string_table[R[30]];
				printf("\t\tMessage is: %s\n", msg.c_str());
			}

		}
		
		else
		{
			throw runtime_error("*** MEMORY ERROR *** : Invalid instruction reached");
		}
		
		PC++;
	}


	
	printf("Execution complete!\n");
	writeResult(cycles, IC);
	return 0;
}

	void writeResult(int instrC, int IC) 
	{
	//cycles == C
	int cycles = instrC;
	//instructionCount == IC
	int instructionCount = IC; 
	//8 * IC / C
	float speedUpRatio = ((8*(float)instructionCount) / (float)cycles);
    FILE *fp;
    fp = fopen("result.txt", "w");

    fprintf(fp, "The total IC count: %d\n", instructionCount);
    fprintf(fp, "The C count: %d\n", cycles);
    fprintf(fp, "The speedup:  %f\n", speedUpRatio);
    

	}

