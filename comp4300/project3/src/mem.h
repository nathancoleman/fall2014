#include "util.h"

#define DATA_SEG_LENGTH 200
#define TEXT_SEG_LENGTH 200

#define DATA_SEG_BASE 0x100000
#define TEXT_SEG_BASE 0x200000

typedef int32 mem_address;
typedef int32 mem_word;

mem_address DATA_TOP = DATA_SEG_BASE;
mem_address TEXT_TOP = TEXT_SEG_BASE;

mem_word DATA_SEG[DATA_SEG_LENGTH];
mem_word TEXT_SEG[TEXT_SEG_LENGTH];

mem_word read(mem_address address);
void write(mem_address address, mem_word data, bool increment_top = false);