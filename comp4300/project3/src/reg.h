#include "util.h"

#define R_LENGTH 32

typedef int32 reg_word;

reg_word R[R_LENGTH];

void init_regs();
reg_word get(int index);
void set(int index, int32 val);