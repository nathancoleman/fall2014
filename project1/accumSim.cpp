/*
Accumulator Simulator
         
By Lucas Saltz
Group 15 with Nathan Coleman
COMP4300 - Computer Architecture
Project 1 - Due 12 September 2014
*/

#include "accumSim.h"



string segment;
string line;
fstream myfile;


int main() {
	
	myfile.open("accumSim.txt");
	if (myfile.is_open()) 
	{
	init();
	//execute();
	}
	myfile.close();	

	return 0;
}

int init() 
{
	//printf ("starting init");
	//myfile.open("test.txt");

	
		//printf ("myfile is open. continuing init");
	//Reading in code line by line
		while (getline (myfile, line)) 
		{

			//If this is the text segment
			if (line == ".text") {
				segment = "text";
			}

			//If this is the data segment
			else if (line == ".data")
				segment = "data";

			//If this is the blank line
			else if (line.length() == 0)
				segment = "";

			//If this is a line within a segment
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

