CC = g++

all: pre-build gprSim

gprSim: pre-build util.o mem.o reg.o gprSim.o
	@$(CC) -o bin/gprSim obj/util.o obj/gprSim.o
gprSim.o: src/gprSim.cpp src/gprSim.h
	@$(CC) -c src/gprSim.cpp -o obj/gprSim.o

util.o: src/util.cpp src/util.h
	@$(CC) -c src/util.cpp -std=gnu++0x -o obj/util.o

mem.o: src/mem.cpp src/mem.h
	@$(CC) -c src/mem.cpp -std=gnu++0x -o obj/mem.o

reg.o: src/reg.cpp src/reg.h
	@$(CC) -c src/reg.cpp -std=gnu++0x -o obj/reg.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz

tar: clean
	@tar -zcf ./group15_project2.tgz *
