// Stack Simulator
// 
// Methods: PUSH X, POP X, ADD, SUB, MULT, DIV
//
// By: Nathan Coleman
// For: Comp 4300 Intro to Computer Architecture

#include "util.h"
#include <stdio.h>
#include <cstdlib>
#include <fstream>

using namespace std;

//#define FILENAME "stackCode.s"

extern mem_addr PC;
extern reg_word R[];
extern mem_addr TEXT_TOP;
extern mem_addr DATA_TOP;
extern mem_addr TOS;
extern mem_word TEXT_SEG[];
extern mem_word DATA_SEG[];
extern mem_word STACK_SEG[];

int init();
int execute();