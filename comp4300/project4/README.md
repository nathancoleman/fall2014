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
- From this directory, ```$ bin/pipeSim filename```


### Data - for lab4c.s
Total Clock Cycles: 
Total Instructions Executed: 1206 
	- 1st iteration: 19
	- 2nd - 99: 12
	- 100th: 11
	Total: 19 + (12*98) + 11 = 1206
Number of NOPs: 204 (200 for the 100 executions and 4 for the end)


##Design Decisions

- We decided to use $0-31 for regular registers and $32-64 for floating point registers. 
- Through research into scoreboard, we decided that it should be its own functioning entity, and not simply a data structure. 
