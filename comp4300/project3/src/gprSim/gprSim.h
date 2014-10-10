#include "../mem/mem.h"
#include "../reg/reg.h"
#include <stdio.h>
#include <stdexcept>
#include <string>

extern std::string FILENAME;

void init();
void execute();
void writeResult(int C, int IC);