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

	int cycles = 0;
	int IC = 0;
	while(PC < TEXT_TOP)
	{
		mem_word instr = read_mem(PC);
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
			mem_address address = R[src];
			printf("\t\tAddress: %x\n", address);
			// TODO!
			mem_word result = read_mem(address);
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

		else if (op == SYSCALL)
		{
			cycles += instruction_totals[SYSCALL];
			IC += 1;
			printf("\tExecuting SYSCALL\n");

			if (R[29] == 8)
			{
				printf("(input string):>");
				std::string input;
				getline(std::cin, input);
				mem_address address = R[30];
				int size = R[31];

				int i;
				for (i = 0; i < input.length(); i++)
				{
					if (i > size)
					{
						break;
					}
					write_mem(address, input[i], false);
					address++;
				}
				write_mem(address, '\n', false);
				address++;
			}
			else if (R[29] == 4)
			{
				std::string msg = string_table[R[30]];
				printf("\t\tMessage is: %s\n", msg.c_str());
			}

		}
		
		else
		{
			throw std::runtime_error("*** MEMORY ERROR *** : Invalid instruction reached");
		}
		
		PC++;
	}


	
	printf("Execution complete!\n");
	writeResult(cycles, IC);
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