#include "util.h"

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

/*
*	This function is used when reading the text segment
*	of the file given as an argument on the command line.
*	It reads in each line and converts it into a 32-bit
*	instruction with the first 8 bits being the binary
*	op-code assigned to the instruction and the remaining
*	24 bits being the address associated with the operation.
*	Address may be a zero value if the instruction does not
*	require an operand.
*/
instruction encode(string line)
{
	instruction instr;
	
	string op = line.substr(0, line.find(" "));
	
	if (op == "PUSH")
	{
		instr = PUSH;
		
		mem_addr address = stoi(line.substr(line.find(" ") + 1), nullptr, 0);
		
		instr = (instr << 24) | address; // Op code is first 8 bits, address last 24 bits
	}

	else if (op == "POP")
	{
		instr = POP;
		
		mem_addr address = stoi(line.substr(line.find(" ") + 1), nullptr, 0);
		
		instr = (instr << 24) | address; // Op code is first 8 bits, address last 24 bits
	}

	else if (op == "ADD")
	{
		instr = ADD;
		
		instr = instr << 24;
	}

	else if (op == "SUB")
	{
		instr = SUB;
		
		instr = instr << 24;
	}

	else if (op == "MULT")
	{
		instr = MULT;
		
		instr = instr << 24;
	}

	else if (op == "DIV")
	{
		instr = DIV;
		
		instr = instr << 24;
	}

	else if (op == "END")
	{
		instr = END;
		
		instr = instr << 24;
	}
		
	return instr;
}

/*
*	This is used to visualize the contents of the various
*	segments at any given time. It takes two arguments, min
*	and max. These refer to the local address of each segment
*	and will frame the output as such.
*
*	printDebug(0,5) will show the first 6 addresses and their
*	contents within each segment
*/
void printDebug(int min, int max)
{
	printf("\n\n```````````````````````````````````````````\nDEBUG INFO:\n");
	
	printf("\tPC -> [%x]\n", PC);
	printf("\tTEXT_TOP -> [%x]\n", TEXT_TOP);
	printf("\tDATA_TOP -> [%x]\n", DATA_TOP);
	printf("\tTOS -> [%x]\n", TOS);
	
	int i;
	printf("\n---TEXT SEGMENT---\n");
	for(i=min; i<=max; i++)
	{
		printf("\t[%x] -> %x\n", TEXT_SEG_BASE + i, TEXT_SEG[i]);
	}
	
	printf("\n---DATA SEGMENT---\n");
	for(i=min; i<=max; i++)
	{
		printf("\t[%x] -> %i\n", DATA_SEG_BASE + i, DATA_SEG[i]);
	}
	
	printf("\n---STACK SEGMENT---\n");
	for(i=min; i<=max; i++)
	{
		printf("\t[%x] -> %i\n", STACK_SEG_BASE + i, STACK_SEG[i]);
	}
	
	printf("\n```````````````````````````````````````````\n\n");
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