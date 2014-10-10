#include "mem.h"

/* Initialize externs */
mem_address PC = TEXT_SEG_BASE;
mem_address DATA_TOP = DATA_SEG_BASE;
mem_address TEXT_TOP = TEXT_SEG_BASE;
mem_word DATA_SEG[DATA_SEG_LENGTH];
mem_word TEXT_SEG[TEXT_SEG_LENGTH];

mem_word read(mem_address address)
{
	if ((address > DATA_SEG_BASE) && (address < TEXT_SEG_BASE))
	{
		mem_address local_address = address - DATA_SEG_BASE;
		
		if (local_address > DATA_SEG_LENGTH)
			throw std::runtime_error("*** MEMORY ERROR *** : Address outside data space");

		return DATA_SEG[local_address];
	}

	else if (address > TEXT_SEG_BASE)
	{
		mem_address local_address = address - TEXT_SEG_BASE;

		if (local_address > TEXT_SEG_LENGTH)
			throw std::runtime_error("*** MEMORY ERROR *** : Address outside text space");

		return TEXT_SEG[local_address];
	}

	else
	{
		throw std::runtime_error("*** MEMORY ERROR *** : Address outside designated segments");
	}

	return 0;
}

void write(mem_address address, mem_word data, bool increment_top)
{

}