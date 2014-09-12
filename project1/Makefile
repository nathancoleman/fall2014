CC = g++

all: accumSim stackSim

accumSim: util.o accumSim.o
	$(CC) -g -o accumSim util.o accumSim.o
accumSim.o: accumSim.cpp
	$(CC) -g -c accumSim.cpp

stackSim: util.o stackSim.o
	$(CC) -g -o stackSim util.o stackSim.o
stackSim.o: stackSim.cpp stackSim.h
	$(CC) -g -c stackSim.cpp

util.o: util.cpp util.h
	$(CC) -g -c util.cpp

clean:
	rm -f stackSim accumSim *.o *.tar.gz

tar: clean
	tar -zcf ./group15_project1.tar.gz *
