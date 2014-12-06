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


### Data
Total Clock Cycles: 
Total Instructions Executed: 
Number of NOPs: 


##Design Decisions

- We decided to use $0-31 for regular registers and $32-47 for floating point registers. 
- Through research into scoreboard, we decided that it should be its own functioning entity, and not simply a data structure. 
