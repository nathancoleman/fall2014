#include "pipeSim.h"

/* Initialize externs */
std::string FILENAME;

if_id_latch if_id;
id_ex_latch id_ex;
ex_mem_latch ex_mem;
mem_wb_latch mem_wb;
int cycle_count = 0, instr_count = 0, nop_count = 0;

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

void clear_latches()
{
	if_id.ir = 0;

	id_ex.op_code = 0;
	id_ex.rs = 0;
	id_ex.rt = 0;
	id_ex.rd = 0;
	id_ex.operand_A = 0;
	id_ex.operand_B = 0;
	id_ex.imm_offset = 0;
	id_ex.new_PC = 0;

	ex_mem.op_code = 0;
	ex_mem.alu_out = 0;
	ex_mem.operand_B = 0;
	ex_mem.rd = 0;

	mem_wb.op_code = 0;
	mem_wb.mdr = 0;
	mem_wb.operand_B = 0;
	mem_wb.alu_out = 0;
	mem_wb.rd = 0;
}

void run()
{
	// if_id_latch if_id;
	// id_ex_latch id_ex;
	// ex_mem_latch ex_mem;
	// mem_wb_latch mem_wb;
	bool user_mode = true;

	printf("Executing...\n");

	clear_latches();

	while (user_mode)
	{
		//clear_latches(&if_id, &id_ex, &ex_mem, &mem_wb);

		if_id = instr_fetch();
		id_ex = instr_decode(if_id);

		// ex_mem = instr_execute(id_ex, &user_mode);

		cycle_count++;

		// mem_wb = mem_access(ex_mem);
		// write_back(mem_wb);

		update_PC();

		if (PC == TEXT_TOP)
			user_mode = false;

		if (!user_mode)
			printf("\tHALTING EXECUTION\n");
	}

	printf("Execution complete!\n");
	printf("\nTOTALS:\n\tCycles: %d\n\tInstructions: %d\n\tNOP: %d\n\n", cycle_count, instr_count, nop_count);
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
	id_ex.op_code = getBits(if_id.ir, 26, 32);
	id_ex.rs = getBits(if_id.ir, 21, 25);
	id_ex.rt = getBits(if_id.ir, 16, 20);
	id_ex.rd = getBits(if_id.ir, 11, 16);
	id_ex.imm_offset = getBits(if_id.ir, 0, 15);
	id_ex.operand_A = R[id_ex.rs];
	id_ex.operand_B = R[id_ex.rt];

	return id_ex;
}

ex_mem_latch instr_execute(id_ex_latch id_ex, bool *user_mode)
{
	printf("\tExecuting instruction...\n");

	ex_mem_latch ex_mem;
	ex_mem.op_code = id_ex.op_code;

	if (ex_mem.op_code == NOP)
		return ex_mem;

	ex_mem.operand_B = id_ex.operand_B;
	ex_mem.rd = id_ex.rd;

	
	switch (ex_mem.op_code)
	{
		case ADD:
			ex_mem.alu_out = id_ex.operand_A + id_ex.operand_B;
			break;

		case ADDI:
			printf("\t\t%x + %d\n", id_ex.operand_A, id_ex.imm_offset);
			ex_mem.alu_out = id_ex.operand_A + id_ex.imm_offset;
			break;

		case LA:
			ex_mem.alu_out = id_ex.imm_offset;
			break;

		case LB:
			printf("\t\t%x\n", id_ex.operand_A);
			ex_mem.alu_out = id_ex.operand_A;
			break;

		case LI:
			ex_mem.alu_out = id_ex.imm_offset;
			break;

		case SUBI:
			ex_mem.alu_out = id_ex.operand_A - id_ex.imm_offset;
			break;

		case SYSCALL:
			if (R[2] == 8)
			{
				printf("\t\t(input string):>");
				string input;
				getline(cin, input);
				int32 address = R[4];
				int size = R[5];

				printf("\t\tAddress is %x, size is %d\n", address, size);

				int i;
				for (i = 0; i < input.length(); i++)
				{
					if (i > size)
					{
						break;
					}
					write_mem(address, input[i], false);
					address++;
				}
				write_mem(address, '\n', false);
				address++;
			}
			else if (R[2] == 4)
			{
				string msg = string_table[R[4]];
				printf("\t\tMessage is: %s\n", msg.c_str());
			}
			else if (R[2] == 10)
			{
				*user_mode = false;
			}
			else if (R[2] == 1)
			{
				printf("\t\t%d\n", R[4]);
			}
			else
			{
				throw std::runtime_error("*** RUNTIME ERROR *** : Unknown SYSCALL type");
			}
			break;

		case NOP:
			break;

		default:
			// This is an invalid op code
			// Do nothing
			break;
	}

	return ex_mem;
}

mem_wb_latch mem_access(ex_mem_latch ex_mem)
{
	printf("\tAccessing memory...\n");

	mem_wb_latch mem_wb;
	mem_wb.op_code = ex_mem.op_code;
	mem_wb.alu_out = ex_mem.alu_out;
	mem_wb.rd = ex_mem.rd;
	mem_wb.operand_B = ex_mem.operand_B;

	switch (mem_wb.op_code)
	{
		case LB:
			printf("\t\tReading from %x\n", ex_mem.alu_out);
			mem_wb.mdr = read_mem(ex_mem.alu_out); // First 8 bits is byte
			break;
	}

	return mem_wb;
}

void write_back(mem_wb_latch mem_wb)
{
	printf("\tWriting back...\n");

	switch (mem_wb.op_code)
	{
		case ADD:
			R[mem_wb.rd] = mem_wb.alu_out;
			break;

		case ADDI:
			printf("\t\tWriting %x to $%d\n", mem_wb.alu_out, mem_wb.rd);
			R[mem_wb.rd] = mem_wb.alu_out;
			break;

		case LA:
			printf("\t\tWriting %x to $%d\n", mem_wb.alu_out, mem_wb.rd);
			R[mem_wb.rd] = mem_wb.alu_out;
			break;

		case LB:
			printf("\t\tWriting %c to $%d\n", mem_wb.mdr, mem_wb.rd);
			R[mem_wb.rd] = mem_wb.mdr;
			break;

		case LI:
			printf("\t\tWriting %d to $%d\n", mem_wb.alu_out, mem_wb.rd);
			R[mem_wb.rd] = mem_wb.alu_out;
			break;

		case SUBI:
			printf("\t\tWriting %x to $%d\n", mem_wb.alu_out, mem_wb.rd);
			R[mem_wb.rd] = mem_wb.alu_out;
			break;
	}
}

void update_PC()
{
	// Init could be modified so that we could increment this by 4
	PC++;
}