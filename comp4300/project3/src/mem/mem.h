#include "../util/util.h"
#include <stdexcept>

#define DATA_SEG_LENGTH 200
#define TEXT_SEG_LENGTH 200
#define DATA_SEG_BASE 0x100000
#define TEXT_SEG_BASE 0x200000

typedef int32 mem_address;
typedef int32 mem_word;

extern mem_address PC;
extern mem_address DATA_TOP;
extern mem_address TEXT_TOP;
extern mem_address DATA_SEG_END;
extern mem_address TEXT_SEG_END;
extern mem_word DATA_SEG[];
extern mem_word TEXT_SEG[];

mem_word read(mem_address address);
void write(mem_address address, mem_word data, bool increment_top = false);