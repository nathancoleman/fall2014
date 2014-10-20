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
	if_id_latch if_id_old;
	if_id_latch if_id_new;
	id_ex_latch id_ex_old;
	id_ex_latch id_ex_new;
	ex_mem_latch ex_mem_old;
	ex_mem_latch ex_mem_new;
	mem_wb_latch mem_wb_old;
	mem_wb_latch mem_wb_new;
	bool run = true;

	printf("Executing...\n");

	while (run)
	{
		if_id_old = if_id_new;
		if_id_new = instr_fetch();
		id_ex_old = id_ex_new;
		id_ex_new = instr_decode(if_id_old);
		ex_mem_old = ex_mem_new;
		ex_mem_new = instr_execute(id_ex_old);
		mem_wb_old = mem_wb_new;
		mem_wb_new = mem_access(ex_mem_old);
		write_back(mem_wb_old);
		update_PC();

		// Update var run here (this is just a placeholder)
		run = false;
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
	id_ex.op_code = if_id.ir >> 26;

	printf("\t\tOp code: %x\n", id_ex.op_code);

	if (id_ex.op_code == ADDI)
	{
		// ADDI rt, rs, imm
		id_ex.rt = (if_id.ir >> 21) & 0x1F;
		id_ex.rs = (if_id.ir >> 16) & 0x1F;
		id_ex.imm_offset = if_id.ir & 0xFFFF;
		printf("id_ex_latch:\n\trt: %x\n\trs: %x\n\timm: %x\n", id_ex.rt, id_ex.rs, id_ex.imm_offset);
	}

	else if (id_ex.op_code == B)
	{

	}

	else if (id_ex.op_code == BEQZ)
	{

	}

	else if (id_ex.op_code == BGE)
	{

	}

	else if (id_ex.op_code == BNE)
	{

	}

	else if (id_ex.op_code == LA)
	{

	}

	else if (id_ex.op_code == LB)
	{

	}

	else if (id_ex.op_code == LI)
	{

	}

	else if (id_ex.op_code == SUBI)
	{

	}

	else if (id_ex.op_code == SYSCALL)
	{

	}

	else
	{
		// This is an invalid op code
		// Do nothing
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