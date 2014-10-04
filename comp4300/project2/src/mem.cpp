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
map<string, int> symbol_table;

//	Function counters
int instr_mem_accesses;
int reg_file_reads;
int alu_ops;
int data_mem_accesses;
int reg_file_writes;

std::map<int, int> instruction_totals;

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
	string op;
	
	// If there is a space, we can use it to
	// find the op, params follow
	if(line.find(" ") != std::string::npos)
	{
		op = line.substr(0, line.find(" "));
	}

	// Without space, we must use something else
	// to find the op - since there are no params
	// the \r immediate follows the op. If the \r
	// is at index 0, this line contains no op
	else
	{
		op = line.substr(0, line.find("\r"));
	}

	printf("\tEncoding instruction\n");
	
	/*
	 *	ADDI Rdest, Rsrc1, Imm
	 */
	if(op == "addi")
	{	
		line = line.substr(line.find("$") + 1);

		int dest = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find("$") + 1);
		
		int src = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		int imm = stoi(line.substr(0, line.find("\r")), 0, 0);

		printf("\t\tADDI: params - %d %d %d\n", dest, src, imm);

		// Encode the instruction
		instr = ADDI;
		instr = instr << 26; // first 6 bits are op code
		instr |= dest << 21; // second 5 bits are dest
		instr |= src << 16; // third 5 bits are src
		instr |= (imm & 0xFFFF); // last 16 bits are immediate

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tDest: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tSrc: %d\n", (instr >> 16) & 0x1F);
		printf("\t\t\tImm: %d\n", instr & 0xFFFF);
	}
	
	/*
	 *	B label
	 */
	else if(op == "b")
	{
		line = line.substr(line.find(" ") + 1);

		string label = line.substr(0, line.find("\r"));

		printf("\t\tB: params - %s\n", label.c_str());

		// Encode the instruction
		instr = B;
		instr = instr << 26; // first 6 bits are op code
		instr |= (symbol_table[label] & 0xFFFF); // last 16 bits are offset

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tOffset: %d\n", instr & 0xFFFF);
	}

	/*
	 *	BEQZ Rsrc1, label
	 */
	else if(op == "beqz")
	{
		line = line.substr(line.find("$") + 1);

		int src = stoi(line.substr(0, line.find(" ")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		string label = line.substr(0, line.find("\r"));

		printf("\t\tBEQZ: params - %d %s\n", src, label.c_str());

		// Encode the instruction
		instr = BEQZ;
		instr = instr << 26; // first 6 bits are op code
		instr |= src << 21; // second 5 bits are src
		instr |= (symbol_table[label] & 0xFFFF); // last 16 bits are offset

		

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tSrc: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tOffset: %d\n", instr & 0xFFFF);
	}

	/*
	 *	BGE Rsrc1, Rsrc2, label
	 */
	else if(op == "bge")
	{
		line = line.substr(line.find("$") + 1);

		int src1 = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find("$") + 1);

		int src2 = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		string label = line.substr(0, line.find("\r"));

		printf("\t\tBGE: params - %d %d %s\n", src1, src2, label.c_str());

		// Encode the instruction
		instr = BGE;
		instr = instr << 26; // first 6 bits are op code
		instr |= src1 << 21; // second 5 bits are src1
		instr |= src2 << 16; // third 5 bits are src2
		instr |= (symbol_table[label] & 0xFFFF); // last 16 bits are offset

		// TODO: Replace the label name with the actual value

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tSrc1: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tSrc2: %d\n", (instr >> 16) & 0x1F);
		printf("\t\t\tOffset: %d\n", instr & 0xFFFF);
	}

	/*
	 *	BNE Rsrc1, Rsrc2, label
	 */
	else if(op == "bne")
	{
		line = line.substr(line.find("$") + 1);

		int src1 = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find("$") + 1);

		int src2 = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		string label = line.substr(0, line.find("\r"));

		printf("\t\tBNE: params - %d %d %s\n", src1, src2, label.c_str());

		// Encode the instruction
		instr = BNE;
		instr = instr << 26; // first 6 bits are op code
		instr |= src1 << 21; // second 5 bits are src1
		instr |= src2 << 16; // third 5 bits are src2
		instr |= (symbol_table[label] & 0xFFFF); // last 16 bits are offset

		// TODO: Replace the label name with the actual value

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tSrc1: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tSrc2: %d\n", (instr >> 16) & 0x1F);
		printf("\t\t\tOffset: %d\n", instr & 0xFFFF);
	}

	/*
	 *	LA Rdest, label
	 */
	else if(op == "la")
	{
		line = line.substr(line.find("$") + 1);

		int dest = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		string label = line.substr(0, line.find("\r"));

		printf("\t\tLA: params - %d %s\n", dest, label.c_str());

		// Encode the instruction
		instr = LA;
		instr = instr << 26; // first 6 bits are op code
		instr |= dest << 21; // second 5 bits are dest

		// TODO: Replace the label name with the actual value

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tDest: %d\n", (instr >> 21) & 0x1F);
	}

	/*
	 *	LB Rdest, offset(address)
	 */
	else if(op == "lb")
	{
		line = line.substr(line.find("$") + 1);

		int dest = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find("$") + 1);

		// Here we get the src for the address of the byte we
		// want to load, then retrieve the address from R[src]

		int src = stoi(line.substr(0, line.find(")")), 0, 0);

		int addr = R[src];

		printf("\t\tLB: params - %d %d\n", dest, addr);

		// Encode the instruction
		instr = LB;
		instr = instr << 26; // first 6 bits are op code
		instr |= dest << 21; // second 5 bits are dest
		instr |= src << 16; // third 5 bits are src

		// TODO: Replace the label name with the actual value

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tDest: %d\n", (instr >> 21) & 0x1F);
	}

	/*
	 *	LI Rdest, Imm
	 */
	else if(op == "li")
	{
		line = line.substr(line.find("$") + 1);

		int dest = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		int imm = stoi(line.substr(0, line.find("\r")), 0, 0);

		printf("\t\tLI: params - %d %d\n", dest, imm);

		// Encode the instruction
		instr = LI;
		instr = instr << 26; // first 6 bits are op code
		instr |= dest << 21; // second 5 bits are dest
		instr |= (imm & 0xFFFF); // last 16 bits are immediate

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tDest: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tImm: %d\n", instr & 0xFFFF);
	}

	/*
	 *	SUBI Rdest, Rsrc1, Imm
	 */
	else if(op == "subi")
	{
		line = line.substr(line.find("$") + 1);

		int dest = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find("$") + 1);

		int src = stoi(line.substr(0, line.find(",")), 0, 0);

		line = line.substr(line.find(" ") + 1);

		int imm = stoi(line.substr(0, line.find("\r")), 0, 0);

		printf("\t\tSUBI: params - %d %d %d\n", dest, src, imm);

		// Encode the instruction
		instr = SUBI;
		instr = instr << 26; // first 6 bits are op code
		instr |= dest << 21; // seconds 5 bits are the dest
		instr |= src << 16; // third 5 bits are the src
		instr |= (imm & 0xFFFF); // last 16 bits are immediate

		printf("\t\t\tOp Code: %x\n", instr >> 26);
		printf("\t\t\tDest: %d\n", (instr >> 21) & 0x1F);
		printf("\t\t\tSrc: %d\n", (instr >> 16) & 0x1F);
		printf("\t\t\tImm: %d\n", instr & 0xFFFF);
	}

	else if(op == "syscall")
	{
		printf("\t\tSYSCALL\n");

		// Encode the instruction
		instr = SYSCALL;
		instr = instr << 26; // first 6 bits are op code
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

void index_symbols()
{
	printf("\tIndexing symbols...\n");

	ifstream file (FILENAME, ios::in);
	string line;
	bool in_text_seg = false;
	int offset = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.find(".text") != std::string::npos)
			{
				in_text_seg = true;
			}
			else
			{
				// If :, we have a symbol
				if (line.find(":") != std::string::npos && in_text_seg)
				{
					string symbol_name = line.substr(0, line.find(":"));
					printf("\t\tFound symbol %s at offset %d\n", symbol_name.c_str(), offset);
					symbol_table[symbol_name] = offset;
				}

				else if(in_text_seg && line.find("\r") != 0)
				{
					offset++;
				}
			}
		}
	}

	printf("\tIndexing COMPLETE... Found %d symbols!\n", (int)symbol_table.size());
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

	index_symbols();
	
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
				//printf("\tProcessing data segment line\n");
			}
			
			/*
			*	If this is a line within the text block,
			*	we need to store the instruction at the next
			*	position in TEXT_SEG
			*/
			else if (segment == "text")
			{
				//printf("\tProcessing text segment line\n");
				

				if(line.find(":") == std::string::npos)
				{
					// We don't want to encode an empty line
					if(line.find("\r") != 0)
					{
						instruction instr = encode(line);
						write(TEXT_TOP, instr);
					}
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
	instruction_totals[ADDI] = 6;
	instruction_totals[B] = 4;
	instruction_totals[BEQZ] = 5;
	instruction_totals[BGE] = 5;
	instruction_totals[BNE] = 5;
	instruction_totals[LA] = 5;
	instruction_totals[LB] = 6;
	instruction_totals[LI] = 3;
	instruction_totals[SUBI] = 6;
	instruction_totals[SYSCALL] = 8;
}