#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstddef>
#include <map>

using namespace std;

//	Instructions
#define ADDI	0x00 //	Add Immediate
#define B 		0x01 //	Branch to target
#define BEQZ	0x02 // Branch on >= 0
#define BGE		0x03 //	Branch on >=
#define BNE		0x04 // Branch on Not Equal
#define LA		0x05 // Load Address
#define LB		0x06 // Load Byte
#define LI 		0x07 // Load Immediate
#define SUBI	0x08 // Subtract Immediate
#define SYSCALL	0x09 // System Call

//	Number of registers we have
#define R_LENGTH 32

//	The size of our data/text segs
#define TEXT_SEG_LENGTH 200
#define DATA_SEG_LENGTH 200

//	The first address in each seg
#define TEXT_SEG_BASE 0x00002000
#define DATA_SEG_BASE 0x00003000

typedef unsigned int int32;

//	Memory types
typedef int32 mem_addr; // Memory address
typedef int32 mem_word; // Memory word


//	Register types
typedef int32 reg_word; // Register word

//	Instruction type
typedef int32 instruction;

//	Functions
instruction encode(string line);
void printDebug(int min, int max);
mem_word read(mem_addr address);
int write(mem_addr address, mem_word data, bool increment_top = true);
void init_instr_table();
int get_instr_op_total(string op_text);
int init();