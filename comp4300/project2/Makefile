CC = g++

all: pre-build accumSim

accumSim: pre-build util.o accumSim.o
	@$(CC) -o bin/accumSim obj/util.o obj/accumSim.o
accumSim.o: src/accumSim.cpp src/accumSim.h
	@$(CC) -c src/accumSim.cpp -o obj/accumSim.o

util.o: src/util.cpp src/util.h
	@$(CC) -c src/util.cpp -std=gnu++0x -o obj/util.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz

tar: clean
	@tar -zcf ./group15_project2.tar.gz *
