#include "../util/util.h"
#include <stdexcept>

#define R_LENGTH 16
#define F_LENGTH 16

typedef int32 reg_word;
typedef float float_word;

extern reg_word R[];
extern float_word F[];

void init_regs();
reg_word read_reg(int index);
float_word read_freg(int index);
void write_reg(int index, reg_word val);
void write_freg(int index, float_word val);
