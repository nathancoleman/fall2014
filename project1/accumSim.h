/*
Accumulator Simulator

By Lucas Saltz
Group 15 with Nathan Coleman
COMP4300 - Computer Architecture
Project 1 - Due 12 September 2014
*/

#include "util.h"
#include <stdio.h>
#include <cstdlib>
#include <fstream>

/*
*	These are shared and are defined in util.cpp
*	to prevent a duplicate symbol clang error
*/
extern mem_addr PC;
extern reg_word R[];
extern mem_addr TEXT_TOP;
extern mem_addr DATA_TOP;
extern mem_addr TOS;
extern mem_word TEXT_SEG[];
extern mem_word DATA_SEG[];
extern mem_word STACK_SEG[];

//Functions
int init();
int execute();
