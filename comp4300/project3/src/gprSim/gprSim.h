#include "../mem/mem.h"
#include "../reg/reg.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <string>

using namespace std;

extern std::string FILENAME;

void init();
void run();

if_id_latch instr_fetch(mem_address *PC);
id_ex_latch instr_decode(if_id_latch if_id_old, mem_address *PC);
ex_mem_latch instr_execute(id_ex_latch id_ex_old);
mem_wb_latch mem_access(ex_mem_latch ex_mem_old);
void write_back(mem_wb_latch mem_wb_old);