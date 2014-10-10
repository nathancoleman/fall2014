#include "../mem/mem.h"
#include "../reg/reg.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <string>

using namespace std;

extern std::string FILENAME;

void init();
void execute();
void writeResult(int C, int IC);