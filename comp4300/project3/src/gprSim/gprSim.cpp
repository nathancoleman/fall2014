#include "gprSim.h"

/* Initialize externs */
std::string FILENAME;

int main(int argc, char* argv[])
{
	if (argc > 1)
		FILENAME = argv[1];
	else
		throw std::runtime_error("*** RUNTIME ERROR *** : Filename expected");

	init();

	run();

	return 0;
}

void init()
{
	init_regs();

	init_segs(FILENAME);

	init_instr_table();
}

void run()
{
	if_id_latch if_id;
	id_ex_latch id_ex;
	ex_mem_latch ex_mem;
	mem_wb_latch mem_wb;
	bool run = true;

	printf("Executing...\n");

	while (run)
	{
		
		if_id = instr_fetch();
		id_ex = instr_decode(if_id);
		ex_mem = instr_execute(id_ex);
		mem_wb = mem_access(ex_mem);
		write_back(mem_wb);
		update_PC();

		if (PC == TEXT_TOP)
		{
			run = false;
			printf("\tHALTING EXECUTION\n");
		}
	}

	printf("Execution complete!\n");
}

if_id_latch instr_fetch()
{
	printf("\tFetching instruction...\n");

	if_id_latch if_id;
	if_id.ir = read_mem(PC);

	return if_id;
}

id_ex_latch instr_decode(if_id_latch if_id)
{
	printf("\tDecoding instruction...\n");

	id_ex_latch id_ex;
	id_ex.op_code = get_op_code(if_id.ir);

	if (is_branch_instr(if_id.ir))
	{
		switch (id_ex.op_code)
		{
			case B:
				id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir);
				printf("\t\tB %x\n", id_ex.new_PC);
				break;

			case BEQZ:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir);
				printf("\t\tBEQZ %x\n", id_ex.new_PC);
				break;

			case BGE:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rt];
				id_ex.operand_B = R[id_ex.rs];
				id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir);
				printf("\t\tBGE %x\n", id_ex.new_PC);
				break;

			case BNE:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rt];
				id_ex.operand_B = R[id_ex.rs];
				id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir);
				printf("\t\tBNE %x\n", id_ex.new_PC);
				break;

			default:
				// This is an invalid op code
				// Do nothing
				break;
		}
	}

	else
	{
		switch (id_ex.op_code)
		{
			case ADDI:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rs];
				id_ex.imm_offset = get_imm(if_id.ir);
				printf("\t\tADDI $%x, %d, %d\n", id_ex.rt, id_ex.operand_A, id_ex.imm_offset);
				break;

			case LA:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.imm_offset = get_imm(if_id.ir);
				printf("\t\tLA $%x, %x\n", id_ex.rt, id_ex.imm_offset);
				break;

			case LB:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rs];
				printf("\t\tLB $%x, %x", id_ex.rt, id_ex.operand_A);
				break;

			case LI:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.imm_offset = get_imm(if_id.ir);
				printf("\t\tLI $%x, %d\n", id_ex.rt, id_ex.imm_offset);
				break;

			case SUBI:
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rs];
				id_ex.imm_offset = get_imm(if_id.ir);
				printf("\t\tSUBI $%x, %d, %d\n", id_ex.rt, id_ex.operand_A, id_ex.imm_offset);
				break;

			case SYSCALL:
				printf("\t\tSYSCALL\n");
				break;

			case NOP:
				printf("\t\tNOP\n");
				break;

			default:
				// This is an invalid op code
				// Do nothing
				break;
		}
	}

	return id_ex;
}

ex_mem_latch instr_execute(id_ex_latch id_ex)
{
	printf("\tExecuting instruction...\n");

	ex_mem_latch ex_mem;

	return ex_mem;
}

mem_wb_latch mem_access(ex_mem_latch ex_mem)
{
	printf("\tAccessing memory...\n");

	mem_wb_latch mem_wb;

	return mem_wb;
}

void write_back(mem_wb_latch mem_wb)
{
	printf("\tWriting back...\n");
}

void update_PC()
{
	// Init could be modified so that we could increment this by 4
	PC++;
}