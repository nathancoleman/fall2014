/*
Accumulator Simulator

By Lucas Saltz
Group 15 with Nathan Coleman
COMP4300 - Computer Architecture
Project 1 - Due 12 September 2014
*/

#include "accumCode.h"

/*
*	These are shared between stackSim and accumSim.
*	They are defined here (as opposed to within the
*	corresponding header file) to avoid a duplicate
*	symbol clang error on compilation.
*/
mem_addr PC = TEXT_SEG_BASE;
reg_word R[R_LENGTH];
mem_addr TEXT_TOP = TEXT_SEG_BASE;
mem_addr DATA_TOP = DATA_SEG_BASE;
mem_addr TOS = STACK_SEG_BASE;
mem_word TEXT_SEG[TEXT_SEG_LENGTH];
mem_word DATA_SEG[DATA_SEG_LENGTH];
mem_word STACK_SEG[STACK_SEG_LENGTH];
string segment;
string line;
fstream myfile;

int main() {
	
	myfile.open("accumSim.txt");
	if (myfile.is_open()) 
	{
	init();
	execute();
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
*	This function is used to simulate a write to memory.
*	It deciphers the address given to determine which segment
*	the caller wishes to store the data in (based on the BASE
*	for each segment) and stores the given data at the address.
*/
int write(mem_addr address, mem_word data)
{
	printf("Writing %x(hex), %i(int) to address %x - ", data, data, address);
	
	bool inTextSeg = (address >= TEXT_SEG_BASE) && (address < DATA_SEG_BASE);
	bool inDataSeg = (address >= DATA_SEG_BASE) && (address < STACK_SEG_BASE);
	bool inStackSeg = (address >= STACK_SEG_BASE);
	
	if (inTextSeg)
	{		
		printf("TEXT SEGMENT\n");
		
		int32 localAdd = address - TEXT_SEG_BASE;
		
		if (localAdd > TEXT_SEG_LENGTH)
		{
			throw runtime_error("*** MEMORY ERROR *** : Address outside available space");
		}
		
		TEXT_SEG[localAdd] = data;
		
		TEXT_TOP++;
	}
	
	else if (inDataSeg)
	{		
		printf("DATA SEGMENT\n");
		
		int32 localAdd = address - DATA_SEG_BASE;
		
		if (localAdd > DATA_SEG_LENGTH)
		{
			throw runtime_error("*** MEMORY ERROR *** : Address outside available space");
		}
			
		DATA_SEG[localAdd] = data;
		
		DATA_TOP++;
	}
	
	else if (inStackSeg)
	{		
		printf("STACK SEGMENT\n");
		
		int32 localAdd = TOS - STACK_SEG_BASE;
		
		if (localAdd > STACK_SEG_LENGTH)
		{
			throw runtime_error("*** MEMORY ERROR *** : Address outside available space");
		}
		
		STACK_SEG[localAdd] = data;
		
		TOS++;
	}
	
	else
	{
		throw runtime_error("*** MEMORY ERROR *** : Address invalid");
	}
	
	return 0;
}

/*
*	This function is used to simulate a read from memory.
*	It deciphers the address given to determine which segment
*	the requested mem_word lies in (based on the BASE for each
*	segment) and returns the data at the given address.
*/
mem_word read(mem_addr address)
{	
	mem_word data;
	
	bool inTextSeg = (address >= TEXT_SEG_BASE) && (address < DATA_SEG_BASE);
	bool inDataSeg = (address >= DATA_SEG_BASE) && (address < STACK_SEG_BASE);
	bool inStackSeg = (address >= STACK_SEG_BASE);
	
	if (inTextSeg)
	{
		mem_addr localAdd = address - TEXT_SEG_BASE;		data = TEXT_SEG[localAdd];
	}
	
	else if (inDataSeg)
	{
		mem_addr localAdd = address - DATA_SEG_BASE;
		data = DATA_SEG[localAdd];
	}
	
	else if (inStackSeg)
	{
		mem_addr localAdd = address - STACK_SEG_BASE;
		data = STACK_SEG[localAdd];
	}
	
	else
	{
		throw runtime_error("*** MEMORY ERROR *** : Address invalid");
	}
	
	return data;
}


instruction encode(string line)
{
	instruction instr;
	
	string op = line.substr(0, line.find(" "));
	
	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits to make room for the address.
	*	Binary OR will give us a 32-bit integer with 8
	*	bits for the op-code and 24 bits for the address
	*	at which the value to be pushed resides.
	*/
	if (op == "LOAD") //change to load
	{
		instr = LOAD;
		
		mem_addr address = stoi(line.substr(line.find(" ") + 1), nullptr, 0);
		
		instr = (instr << 24) | address;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits to make room for the address.
	*	Binary OR will give us a 32-bit integer with 8
	*	bits for the op-code and 24 bits for the address
	*	to which the popped value is to be assigned.
	*/
	else if (op == "STORE") //change to store
	{
		instr = STORE;

		mem_addr address = stoi(line.substr(line.find(" ") + 1), nullptr, 0);

		instr = (instr << 24) | address;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits, but only to make this consistent
	*	with our other 32-bit instructions (where the first
	*	8 bits are the op-code). The address portion will
	*	have a zero value since no operand is required for ADD
	*	to operate on stack values.
	*/
	else if (op == "ADD")
	{
		instr = ADD;
		instr = instr << 24;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits, but only to make this consistent
	*	with our other 32-bit instructions (where the first
	*	8 bits are the op-code). The address portion will
	*	have a zero value since no operand is required for SUB
	*	to operate on stack values.
	*/
	else if (op == "SUB")
	{
		instr = SUB;
		instr = instr << 24;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits, but only to make this consistent
	*	with our other 32-bit instructions (where the first
	*	8 bits are the op-code). The address portion will
	*	have a zero value since no operand is required for MULT
	*	to operate on stack values.
	*/
	else if (op == "MULT")
	{
		instr = MULT;
		instr = instr << 24;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits, but only to make this consistent
	*	with our other 32-bit instructions (where the first
	*	8 bits are the op-code). The address portion will
	*	have a zero value since no operand is required for DIV
	*	to operate on stack values.
	*/
	else if (op == "DIV")
	{
		instr = DIV;
		instr = instr << 24;
	}

	/*
	*	Here we need to shift the instruction code
	*	left by 24 bits, but only to make this consistent
	*	with our other 32-bit instructions (where the first
	*	8 bits are the op-code). The address portion will
	*	have a zero value since no operand is required for END.
	*/
	else if (op == "END")
	{
		instr = END;
		instr = instr << 24;
	}
		
	return instr;
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
		
		else if (op == STORE) //Was POP
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

			/*
			*	Reset the data at this position since
			*	we are taking two from the stack and
			*	only returning the result
			*/
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
