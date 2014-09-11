/*
Accumulator Simulator

By Lucas Saltz
Group 15 with Nathan Coleman
COMP4300 - Computer Architecture
Project 1 - Due 12 September 2014
*/


#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;


#define ADD 	0b10101010
#define SUB 	0b01010101
#define MULT 	0b00001111
#define DIV		0b11110000
#define LOAD	0b11111111
#define STORE	0b00000000
#define END		0b10100101


#define R_LENGTH 8

#define TEXT_SEG_LENGTH 200
#define DATA_SEG_LENGTH 200
#define STACK_SEG_LENGTH 200

#define TEXT_SEG_BASE 0x00200000
#define DATA_SEG_BASE 0x00300000
#define STACK_SEG_BASE 0x00400000


typedef unsigned int int32;
// Memory 
typedef int32 mem_addr; // Memory address
typedef int32 mem_word; // Memory word
typedef int32 instruction;
typedef int32 reg_word; // Register word


//Functions
int write(mem_addr address, mem_word data);
instruction encode(string line);
void printDebug(int min, int max);
mem_word read(mem_addr address);
int init();
int execute();
