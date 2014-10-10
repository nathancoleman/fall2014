#include "../util/util.h"
#include <stdexcept>

#define R_LENGTH 32

typedef int32 reg_word;

extern reg_word R[];

void init_regs();
reg_word get(int index);
void set(int index, int32 val);