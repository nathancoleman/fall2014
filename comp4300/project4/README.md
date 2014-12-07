## Group Info
- Group #: 15
- Members:
    - Nathan Coleman (ndc0002)
    - Lucas Saltz (las0029)

## Notes
- Runs only on UNIX systems

## Instructions
### Compile
This project includes a Makefile and requires g++ for compilation
- From this directory, run ```make```
- To force rebuild of all targets, run ```make -B``` or ```make --always-make```
- To clean make-related files, run ```make clean```

### Run
After running ```$ make```:
- From this directory, ```$ bin/scoreboard filename```


### Data - for lab4c.s
From the lab, we used 6 cycles for the execution of the fmul, 2 cycles for the execution of the LD, LI, SD, ADDI, and branch executions. 
Total Clock Cycles: 			4835
	- 1st iteration:		80
	- 2nd-99th iteration:	48
	- 100th iteration:		51
Total Instructions Executed: 	1206 
	- 1st iteration:		19
	- 2nd - 99:				12
	- 100th:				11
	Total: 19 + (12*98) + 11 = 1206
Number of NOPs: 204 (200 for the 100 executions and 4 for the end)


### Design Decisions

- We decided to use $0-15 for regular registers and $f0-f15 for floating point registers. 
- Through research into scoreboard, we decided that it should be its own functioning entity, and not simply a data structure. 
	- Scoreboard was implemented using a 2D Integer array for the Functinal Unit Status:
		- unsigned int fu_status[5][9];
			- Where [5] represents 
				INTEGER_FU, MULT1_FU, MULT2_FU, ADD_FU, DIVIDE_FU
			- Where [9] represents
				BUSY, OP, FI, FJ, FK, QJ, QK, RJ, RK

### About the program
The program is not fully functional, it can parse the file, issue the instructions, has the functionality of the Scoreboard built out and has the structure for handling WAW, structural hazards and RAW as well. 

