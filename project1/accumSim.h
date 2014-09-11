/*
*	Accumulator Simulator
*	by: Lucas Saltz
*	for: COMP4300 Fall 2014
*
*	This simulates an instruction set for MIPS using an accumulator
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
extern string FILENAME;

//Functions
int init();
int execute();
