#ifndef UTIL
#define UTIL

#define ADDI	0x01 //	Add Immediate
#define B 		0x02 //	Branch to target
#define BEQZ	0x03 // Branch on >= 0
#define BGE		0x04 //	Branch on >=
#define BNE		0x05 // Branch on Not Equal
#define LA		0x06 // Load Address
#define LB		0x07 // Load Byte
#define LI 		0x08 // Load Immediate
#define SUBI	0x09 // Subtract Immediate
#define SYSCALL	0x0A // System Call
#define NOP		0x0B // Null Op
#define ADD 	0x0C // Add
#define LD 		0x0D // Load Double
#define SD		0x0E // Store Double
#define FADD	0x0F // Floating Point Add
#define FSUB	0x10 // Floating Point Sub
#define FMUL	0x11 // Floating Point Mul

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

int32 getBits(instruction instr, int32 min, int32 max);
bool is_branch_instr(int32 op_code);

#endif