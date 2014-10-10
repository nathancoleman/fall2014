CC = g++

all: pre-build gprSim

gprSim: pre-build mem.o gprSim.o
	@$(CC) -o bin/gprSim obj/mem.o obj/gprSim.o
gprSim.o: src/gprSim.cpp src/gprSim.h
	@$(CC) -c src/gprSim.cpp -o obj/gprSim.o

mem.o: src/mem.cpp src/mem.h
	@$(CC) -c src/mem.cpp -std=gnu++0x -o obj/mem.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz

tar: clean
	@tar -zcf ./group15_project2.tgz *
