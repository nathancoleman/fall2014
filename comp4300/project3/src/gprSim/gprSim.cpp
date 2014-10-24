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
	id_ex.op_code = get_op_code(if_id.ir);

	if (is_branch_instr(if_id.ir))
	{
		switch (id_ex.op_code)
		{
			case B:
				printf("\t\tB to offset %x\n", TEXT_SEG_BASE + get_imm(if_id.ir));
				id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir) - 1;
				break;

			case BEQZ:
				printf("\t\tBEQZ to offset %x possible\n", TEXT_SEG_BASE + get_imm(if_id.ir));
				id_ex.rt = get_rt(if_id.ir);
				if (R[id_ex.rt] == 0)
					id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir) - 1;
				break;

			case BGE:
				printf("\t\tBGE to offset %x possible\n", TEXT_SEG_BASE + get_imm(if_id.ir));
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rt];
				id_ex.operand_B = R[id_ex.rs];
				if (id_ex.operand_A >= id_ex.operand_B)
					id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir) -1;
				break;

			case BNE:
				printf("\t\tBNE to offset %x possible\n", TEXT_SEG_BASE + get_imm(if_id.ir));
				id_ex.rt = get_rt(if_id.ir);
				id_ex.rs = get_rs(if_id.ir);
				id_ex.operand_A = R[id_ex.rt];
				id_ex.operand_B = R[id_ex.rs];
				if (id_ex.operand_A != id_ex.operand_B)
					id_ex.new_PC = TEXT_SEG_BASE + get_imm(if_id.ir) -1;
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