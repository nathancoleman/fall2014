#ifndef UTIL
#define UTIL

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
#define NOP		0x0A // Null Op

typedef unsigned int int32;
typedef int32 instruction;

struct if_id_latch
{
	instruction ir;
};

struct id_ex_latch
{
	int32 op_code;
	int32 rs, rt, rd;
	int32 operand_A, operand_B;
	int32 imm_offset;
	int32 new_PC;
};

struct ex_mem_latch
{
	int32 op_code;
	int32 alu_out;
	int32 operand_B;
	int32 rd;
};

struct mem_wb_latch
{
	int32 op_code;
	int32 mdr;
	int32 operand_B;
	int32 alu_out;
	int32 rd;
};

bool is_branch_instr(instruction instr);

#endif