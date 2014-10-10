#include "mem.h"

/* Initialize externs */
mem_address PC = TEXT_SEG_BASE;
mem_address DATA_TOP = DATA_SEG_BASE;
mem_address TEXT_TOP = TEXT_SEG_BASE;
mem_word DATA_SEG[DATA_SEG_LENGTH];
mem_word TEXT_SEG[TEXT_SEG_LENGTH];

mem_word read(mem_address address)
{
	return 0;
}

void write(mem_address address, mem_word data, bool increment_top)
{

}