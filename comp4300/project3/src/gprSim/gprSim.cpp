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
	if_id_new.ir = NOP; // Using pull method, so first instruction will be nothing
	if_id_new.ir = if_id_new.ir << 26;
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
		if (PC > TEXT_TOP)
		{
			run = false;
			printf("STOP RUNNING!\n");
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
	id_ex.op_code = (if_id.ir >> 26);
	id_ex.rt = (if_id.ir >> 21);
	id_ex.rs = (if_id.ir >> 16) & 0x1F;
	id_ex.imm_offset = if_id.ir & 0xFFFF;
	printf("\t\top: %x\n\t\trt: %x\n\t\trs: %x\n\t\timm: %x\n", id_ex.op_code, id_ex.rt, id_ex.rs, id_ex.imm_offset);

	if (is_branch_instr(if_id.ir))
	{
		printf("BRANCH INSTRUCTION FOUND\n");
	}

	else
	{
		switch (id_ex.op_code)
		{
			case ADDI:
				break;

			case LA:
				break;

			case LB:
				break;

			case LI:
				break;

			case SUBI:
				break;

			case SYSCALL:
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