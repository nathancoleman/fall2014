CC = g++

all: pre-build pipeSim

pipeSim: pre-build util.o mem.o reg.o pipeSim.o
	@$(CC) -o bin/pipeSim obj/util.o obj/mem.o obj/reg.o obj/pipeSim.o
pipeSim.o: src/pipeSim/pipeSim.cpp src/pipeSim/pipeSim.h
	@$(CC) -c src/pipeSim/pipeSim.cpp -std=gnu++0x -o obj/pipeSim.o

util.o: src/util/util.cpp src/util/util.h
	@$(CC) -c src/util/util.cpp -std=gnu++0x -o obj/util.o

mem.o: src/mem/mem.cpp src/mem/mem.h
	@$(CC) -c src/mem/mem.cpp -std=gnu++0x -o obj/mem.o

reg.o: src/reg/reg.cpp src/reg/reg.h
	@$(CC) -c src/reg/reg.cpp -std=gnu++0x -o obj/reg.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj *.tar.gz *.tgz

force:
	@make --always-make

tar.gz: clean
	@tar -zcf ./group15_project3.tar.gz *

tgz: clean
	@tar -zcf ./group15_project3.tgz *
