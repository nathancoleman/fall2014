CC = gcc

all: pre-build ServerUDP ClientTCP

ServerUDP: pre-build ServerUDP.o
	@$(CC) -o bin/ServerUDP obj/ServerUDP.o
ServerUDP.o: src/ServerUDP.c
	@$(CC) -c src/ServerUDP.c -o obj/ServerUDP.o

ClientTCP: pre-build ClientTCP.o
	@$(CC) -o bin/ClientTCP obj/ClientTCP.o
ClientTCP.o: src/ClientTCP.c
	@$(CC) -c src/ClientTCP.c -o obj/ClientTCP.o

pre-build:
	@mkdir -p bin obj

tar: clean
	@tar -zcf ./lab120.tar.gz *

zip: clean
	@zip -r ./lab120 ./

clean:
	@rm -rf bin obj *.tar.gz *.zip