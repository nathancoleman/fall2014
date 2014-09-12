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
#### All
- From this directory, run ```make```
- To force rebuild of all targets, run ```make -B``` or ```make --always-make```
- To clean make-related files, run ```make clean```

#### stackSim
- From this directory, run ```make stackSim```
- To force rebuild of target, run ```make -B stackSim``` or ```make --always-make stackSim```

#### accumSim
- From this directory, run ```make accumSim```
- To force rebuild of target, run ```make -B accumSim``` or ```make --always-make accumSim```

### Run
After running ```make```, ```make stackSim```, or ```make accumSim```:
- stackSim:
    - From this directory, ```bin/stackSim filename```
    - The stackCode file is included as example input. To run, ```bin/stackSim stackCode```
- accumSim:
    - From this directory, ```bin/accumSim filename```
    - The accumCode file is included as example input. To run, ```bin/accumSim accumCode```
