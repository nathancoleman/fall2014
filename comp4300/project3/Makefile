CC = g++

all: pre-build gprSim

gprSim: pre-build util.o mem.o reg.o gprSim.o
	@$(CC) -o bin/gprSim obj/util.o obj/mem.o obj/reg.o obj/gprSim.o
gprSim.o: src/gprSim/gprSim.cpp src/gprSim/gprSim.h
	@$(CC) -c src/gprSim/gprSim.cpp -o obj/gprSim.o

util.o: src/util/util.cpp src/util/util.h
	@$(CC) -c src/util/util.cpp -std=gnu++0x -o obj/util.o

mem.o: src/mem/mem.cpp src/mem/mem.h
	@$(CC) -c src/mem/mem.cpp -std=gnu++0x -o obj/mem.o

reg.o: src/reg/reg.cpp src/reg/reg.h
	@$(CC) -c src/reg/reg.cpp -std=gnu++0x -o obj/reg.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz

tar: clean
	@tar -zcf ./group15_project2.tgz *
