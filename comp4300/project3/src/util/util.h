#include <string>

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

typedef unsigned int int32;
typedef int32 instruction;

instruction encode_instruction(std::string line);