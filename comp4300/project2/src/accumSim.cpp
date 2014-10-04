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

	int IC = 0;
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 26;
		// printf("%s\n", "Inside the while loop");


		if (op == ADDI) 
		{
			IC += 6;
			printf("\tExecuting ADDI\n");
			int dest =  (instr >> 21) & 0x1F; 
			int src = (instr >> 16) & 0x1F;
			int imm = instr & 0xFFFF;

			R[dest] = R[src] + imm; //Registers array 

			printf("\t\tRESULT: %d\n", R[dest]);
		}
		
		else if (op == B) 
		{
			IC += 4;
			printf("\tExecuting B\n");

			int offset = instr & 0xFFFF;

			printf("\t\tMoving to offset %d\n", offset);
			PC = TEXT_SEG_BASE + offset - 1; // Account for increment at the end of this loop
		}
		
		else if (op == BEQZ)
		{
			printf("\tExecuting BEQZ\n");
			IC += 5;
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
			IC += 5;
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
			IC += 5;
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
			IC += 5;
			printf("\tExecuting LA\n");

			int dest = (instr >> 21) & 0x1F;
			int address = instr & 0xFFFF;

			R[dest] = address;
		}

		else if (op == LB)
		{
			IC += 6;
			printf("\tExecuting LB\n");

			int dest = (instr >> 21) & 0x1F;
			int src = (instr >> 16) & 0x1F;
			mem_addr address = R[src];
			printf("\t\tAddress: %x\n", address);
			// TODO!
			// mem_word result = read(address);
			// result = result & 0xFF; // First 8 bits is byte at address
			// printf("\t\tResult: %d\n", result);
		}

		else if (op == LI)
		{
			IC += 3;
			printf("\tExecuting LI\n");

			int dest = (instr >> 21) & 0x1F;
			int imm = instr & 0xFFFF; //grabbing last 16bits for imm value???
			// TODO!
			//R[dest] = imm;
		}

		else if (op == SUBI)
		{
			IC += 6;
			printf("\tExecuting SUBI\n");

			int dest = (instr >> 21) & 0x1F;
			int src = (instr >> 16) & 0x1F;

			int imm; 
			imm = instr & 0xFFFF; //grabbing last 16 bits?

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
			IC += 8;
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
	writeResult(IC);
	return 0;
}

	void writeResult(int instrCount) 
	{
/*FILE *fp;
char* str = "string";
int x = 10;

fp=fopen("test.txt", "w");
if(fp == NULL)
    exit(-1);
fprintf(fp, "This is a string which is written to a file\n");
fprintf(fp, "The string has %d words and keyword %s\n", x, str);
fclose(fp);*/
	int IC = instrCount;
	int speedUpRatio = (8*IC) / 1;// Replace 1 with C
    FILE *fp;
    fp = fopen("result.txt", "w");

    fprintf(fp, "The total IC count: %d\n", IC);


	}

