#include "stackSim.h"

string FILENAME;

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
	
	printDebug(0,5);
	
	return 0;
}

int init()
{
	printf("Initializing...\n");
	
	// Read in file
	string line;
	ifstream file (FILENAME, ios::in);
	
	if (file.is_open())
	{
		string segment;
		
		while (getline(file, line))
		{
			// If this is the text segment
			if (line == ".text")
			{
				segment = "text";
			}
				
			// If this is the data segment
			else if (line == ".data")
			{
				segment = "data";
			}
				
			// If this is the blank seperator line
			else if (line.length() == 0)
			{
				segment = "";
			}
			
			// If this is a line within a segment
			else if (segment == "data")
			{
				mem_addr address = stoi(line.substr(0, line.find(":")), nullptr, 0);
				mem_word data = stoi(line.substr(line.find(":") + 1));
				
				write(address, data);
			}
			
			else if (segment == "text")
			{
				instruction instr = encode(line);
				
				write(TEXT_TOP, instr);
			}		
		
		}
				
		file.close();
	}
	else
	{
		throw runtime_error("Failed to open input file");
	}

	printf("Initialization complete!\n");
	return 0;
}

int execute()
{	
	printf("Executing...\n");
	
	while(PC < TEXT_TOP)
	{
		mem_word instr = read(PC);
		int32 op = instr >> 24;
		mem_addr address = instr & ((1 << 24) - 1);
		
		//printf("%x - ", op);
		
		
		if (op == PUSH)
		{
			printf("PUSH [%x]\n", address);
			
			write(TOS, read(address));
		}
		
		else if (op == POP)
		{
			printf("POP [%x]\n", address);
			
			mem_word val1 = read(--TOS);
			STACK_SEG[TOS - STACK_SEG_BASE] = 0; // Reset data
			
			write(address, val1);	
		}
		
		else if (op == ADD)
		{
			printf("ADD\n");
			
			mem_word val1 = read(--TOS);
			STACK_SEG[TOS - STACK_SEG_BASE] = 0; // Reset data
			mem_word val2 = read(--TOS);
						
			write(TOS, val2 + val1);	
		}
		
		else if (op == SUB)
		{
			printf("SUB\n");
			
			mem_word val1 = read(--TOS);
			STACK_SEG[TOS - STACK_SEG_BASE] = 0; // Reset data
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 - val1);	
		}
		
		else if (op == MULT)
		{
			printf("MULT\n");
	
			mem_word val1 = read(--TOS);
			STACK_SEG[TOS - STACK_SEG_BASE] = 0; // Reset data
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 * val1);
		}
		
		else if (op == DIV)
		{
			printf("DIV\n");
						
			mem_word val1 = read(--TOS);
			STACK_SEG[TOS - STACK_SEG_BASE] = 0; // Reset data
			mem_word val2 = read(--TOS);
			
			write(TOS, val2 / val1);	
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