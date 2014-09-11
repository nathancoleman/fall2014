#include <string>
#include <iostream>

using namespace std;

#define ADD 	0b10101010
#define SUB 	0b01010101
#define MULT 	0b00001111
#define DIV		0b11110000
#define PUSH	0b11111111
#define POP		0b00000000
#define END		0b10100101

#define R_LENGTH 8

#define TEXT_SEG_LENGTH 200
#define DATA_SEG_LENGTH 200
#define STACK_SEG_LENGTH 200

#define TEXT_SEG_BASE 0x00200000
#define DATA_SEG_BASE 0x00300000
#define STACK_SEG_BASE 0x00400000

typedef unsigned int int32;

/* Memory */
typedef int32 mem_addr; // Memory address
typedef int32 mem_word; // Memory word
// mem_addr PC = TEXT_SEG_BASE;

/* Registers */
typedef int32 reg_word; // Register word
// reg_word R[R_LENGTH];

/* Segments */
typedef int32 instruction;
// mem_addr TEXT_TOP = TEXT_SEG_BASE;
// mem_addr DATA_TOP = DATA_SEG_BASE;
// mem_addr TOS = STACK_SEG_BASE;
// mem_word TEXT_SEG[TEXT_SEG_LENGTH];
// mem_word DATA_SEG[DATA_SEG_LENGTH];
// mem_word STACK_SEG[STACK_SEG_LENGTH];


// Functions
instruction encode(string line);
void printDebug(int min, int max);
mem_word read(mem_addr address);
int write(mem_addr address, mem_word data);