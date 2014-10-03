#include "mem.h"

string FILENAME;

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
mem_word TEXT_SEG[TEXT_SEG_LENGTH];
mem_word DATA_SEG[DATA_SEG_LENGTH];

//	Function counters
int instr_mem_accesses;
int reg_file_reads;
int alu_ops;
int data_mem_accesses;
int reg_file_writes;

struct instr_table_line
{
	string instr;
	int32 instr_mem_accesses;
	int32 reg_file_reads;
	int32 alu_ops;
	int32 data_mem_accesses;
	int32 reg_file_writes;
	int32 total_ops;
};

struct instr_table_line instr_table[10];

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

	if(line.find(" ") != std::string::npos)
	{
		// This must be an instruction with no param
		// such as SYSCALL
		instr = SYSCALL;
		return instr;
	}
	
	string op = line.substr(line.find("\t") + 1, line.find(" "));

	printf("OP is %s | line is now: %s\n", op.c_str(), line.c_str());
	
	/*
	 *	ADDI Rdest, Rsrc1, Imm
	 */
	if(op == "addi")
	{
		instr = ADDI;

	}
	
	/*
	 *	B label
	 */
	else if(op == "b")
	{
		instr = B;
	}

	/*
	 *	BEQZ Rsrc1, label
	 */
	else if(op == "beqz")
	{
		instr = BEQZ;
	}

	/*
	 *	BGE Rsrc1, Rsrc2, label
	 */
	else if(op == "bge")
	{
		instr = BGE;
	}

	/*
	 *	BNE Rsrc1, Rsrc2, label
	 */
	else if(op == "bne")
	{
		instr = BNE;
	}

	/*
	 *	LA Rdest, label
	 */
	else if(op == "la")
	{
		instr = LA;
	}

	/*
	 *	LB Rdest, label
	 */
	else if(op == "lb")
	{
		instr = LB;
	}

	/*
	 *	LI Rdest, Imm
	 */
	else if(op == "li")
	{
		instr = LI;
	}

	/*
	 *	SUBI Rdest, Rsrc1, Imm
	 */
	else if(op == "subi")
	{
		instr = SUBI;
	}

	/*
	 *	SYSCALL
	 */
	else if(op == "syscall")
	{
		instr = SYSCALL;
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
	bool inDataSeg = (address >= DATA_SEG_BASE);
	
	if (inTextSeg)
	{
		mem_addr localAdd = address - TEXT_SEG_BASE;
		data = TEXT_SEG[localAdd];
	}
	
	else if (inDataSeg)
	{
		mem_addr localAdd = address - DATA_SEG_BASE;
		data = DATA_SEG[localAdd];
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
int write(mem_addr address, mem_word data, bool increment_top)
{	
	bool inTextSeg = (address >= TEXT_SEG_BASE) && (address < DATA_SEG_BASE);
	bool inDataSeg = (address >= DATA_SEG_BASE);
	
	if (inTextSeg)
	{				
		int32 localAdd = address - TEXT_SEG_BASE;
		
		if (localAdd > TEXT_SEG_LENGTH)
		{
			throw runtime_error("*** MEMORY ERROR *** : Address outside available space");
		}
		
		TEXT_SEG[localAdd] = data;
		
		if (increment_top)
		{
			TEXT_TOP++;
		}
	}
	
	else if (inDataSeg)
	{				
		int32 localAdd = address - DATA_SEG_BASE;
		
		if (localAdd > DATA_SEG_LENGTH)
		{
			throw runtime_error("*** MEMORY ERROR *** : Address outside available space");
		}
			
		DATA_SEG[localAdd] = data;
		
		if (increment_top)
		{
			DATA_TOP++;
		}
	}
	
	else
	{
		throw runtime_error("*** MEMORY ERROR *** : Address invalid");
	}
	
	return 0;
}

/*
*	This is the "all-powerful initialization routine".
*	Reads in and initializes the data segment and the
*	text segment from the file given as an argument on
*	the command line
*/
int init()
{
	printf("Initializing...\n");
	
	string line;
	ifstream file (FILENAME, ios::in);
	
	if (file.is_open())
	{
		string segment;
		
		while (getline(file, line))
		{
			if (line == ".text\r")
			{
				printf("Setting segment to text\n");
				segment = "text";
			}
				
			else if (line == ".data\r")
			{
				printf("Setting segment to data\n");
				segment = "data";
			}
				
			else if (line.length() == 0)
			{
				// Do nothing
			}
			
			/*
			*	If this is a line within the data block,
			*	we need to store the data at the given
			*	address in DATA_SEG
			*/
			else if (segment == "data")
			{
				printf("\tProcessing data segment line\n");
			}
			
			/*
			*	If this is a line within the text block,
			*	we need to store the instruction at the next
			*	position in TEXT_SEG
			*/
			else if (segment == "text")
			{
				printf("\tProcessing text segment line\n");
				

				if(line.find(":") == std::string::npos)
				{
					instruction instr = encode(line);
					write(TEXT_TOP, instr);
				}
				else
				{
					// This is a label
				}
			}		
		
		}
				
		file.close();
	}
	else
	{
		throw runtime_error("Failed to open input file");
	}

	init_instr_table();

	printf("Initialization complete!\n");
	return 0;
}

void init_instr_table()
{
	int i;
	int instr_table_len = sizeof(instr_table)/sizeof(instr_table[0]);

	for(i = 0; i < instr_table_len; i++)
	{
		instr_table_line line;

		switch(i)
		{
			case ADDI:
				line.instr = "addi";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 1;
				line.total_ops = 6;
				break;

			case B:
				line.instr = "b";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 0;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 0;
				line.total_ops = 4;
				break;

			case BEQZ:
				line.instr = "beqz";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 0;
				line.total_ops = 5;
				break;

			case BGE:
				line.instr = "bge";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 0;
				line.total_ops = 5;
				break;

			case BNE:
				line.instr = "bne";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 0;
				line.total_ops = 5;
				break;

			case LA:
				line.instr = "la";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 0;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 1;
				line.total_ops = 5;
				break;

			case LB:
				line.instr = "lb";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 1;
				line.total_ops = 6;
				break;

			case LI:
				line.instr = "li";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 0;
				line.alu_ops = 0;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 1;
				line.total_ops = 3;
				break;

			case SUBI:
				line.instr = "subi";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 0;
				line.reg_file_writes = 1;
				line.total_ops = 6;
				break;

			case SYSCALL:
				line.instr = "syscall";
				line.instr_mem_accesses = 2;
				line.reg_file_reads = 1;
				line.alu_ops = 2;
				line.data_mem_accesses = 2;
				line.reg_file_writes = 1;
				line.total_ops = 8;
				break;
		}
	}	
}

int get_instr_op_total(string op_text)
{
	int total = 0;
	int i;
	int instr_table_len = sizeof(instr_table)/sizeof(instr_table[0]);

	for(i = 0; i < instr_table_len; i++)
	{
		if(instr_table[i].instr == op_text)
		{
			total = instr_table[i].total_ops;
			break;
		}
	}

	return total;
}