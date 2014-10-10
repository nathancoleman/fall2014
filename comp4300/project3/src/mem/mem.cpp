#include "mem.h"

/* Initialize externs */
mem_address PC = TEXT_SEG_BASE;
mem_address DATA_TOP = DATA_SEG_BASE;
mem_address TEXT_TOP = TEXT_SEG_BASE;
mem_address DATA_SEG_END = DATA_SEG_BASE + DATA_SEG_LENGTH;
mem_address TEXT_SEG_END = TEXT_SEG_BASE + TEXT_SEG_LENGTH;
mem_word DATA_SEG[DATA_SEG_LENGTH];
mem_word TEXT_SEG[TEXT_SEG_LENGTH];

mem_word read(mem_address address)
{
	if (DATA_SEG_BASE < address < DATA_SEG_END)
	{
		mem_address local_address = address - DATA_SEG_BASE;
		return DATA_SEG[local_address];
	}

	else if (TEXT_SEG_BASE < address < TEXT_SEG_END)
	{
		mem_address local_address = address - TEXT_SEG_BASE;
		return TEXT_SEG[local_address];
	}

	else
	{
		throw std::runtime_error("*** MEMORY ERROR *** : Address outside designated segment space");
	}

	return 0;
}

void write(mem_address address, mem_word data, bool increment_top)
{
	if (DATA_SEG_BASE < address < DATA_SEG_END)
	{
		mem_address local_address = address - DATA_SEG_BASE;
		DATA_SEG[local_address] = data;
	}

	else if (TEXT_SEG_BASE < address < TEXT_SEG_END)
	{
		mem_address local_address = address - TEXT_SEG_BASE;
		TEXT_SEG[local_address] = data;
	}

	else
	{
		throw std::runtime_error("*** MEMORY ERROR *** : Address outside designated segment space");	
	}
}