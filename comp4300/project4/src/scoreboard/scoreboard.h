#include "../reg/reg.h"
#include "../mem/mem.h"
#include "../util/util.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <string>

using namespace std;

extern string FILENAME;

#define ISSUE 		0
#define READ_OPS	1
#define EXEC_COMP	2
#define WRITE_RES	3

#define INTEGER_FU	0
#define MULT1_FU 	1
#define MULT2_FU	2
#define ADD_FU		3
#define DIVIDE_FU	4

#define BUSY 		0
#define OP 		 	1
#define FI 		 	2
#define FJ 		 	3
#define FK 		 	4
#define QJ 		 	5
#define QK 		 	6
#define RJ 		 	7
#define RK 		 	8

#define UNUSED		99

unsigned int instr_status[TEXT_SEG_LENGTH][4];
unsigned int fu_status[5][9];
unsigned int res_status[R_LENGTH + F_LENGTH];

void init();
void init_res_status();
void run();

bool can_issue_instr(instruction instr);
bool check_for_waw(instruction instr);
bool check_for_str_haz(instruction instr);
void issue_instr(instruction instr);

void print_instr_status_board();
void print_fu_status_board();
void print_res_status_board();
