CC = g++

all: pre-build accumSim

accumSim: pre-build mem.o accumSim.o
	@$(CC) -o bin/accumSim obj/mem.o obj/accumSim.o
accumSim.o: src/accumSim.cpp src/accumSim.h
	@$(CC) -c src/accumSim.cpp -o obj/accumSim.o

mem.o: src/mem.cpp src/mem.h
	@$(CC) -c src/mem.cpp -std=gnu++0x -o obj/mem.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz

tar: clean
	@tar -zcf ./group15_project2.tar.gz *
