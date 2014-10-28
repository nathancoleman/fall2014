CC = gcc

all: pre-build UDPClient

UDPClient: pre-build UDPClient.o
	@$(CC) -o bin/UDPClient obj/UDPClient.o
UDPClient.o: src/UDPClient.c
	@$(CC) -c src/UDPClient.c -o obj/UDPClient.o

pre-build:
	@mkdir -p bin obj

tar: clean
	@tar -zcf ./lab220.tar.gz *

zip: clean
	@zip -r ./lab220 ./

clean:
	@rm -rf bin obj *.tar.gz *.zip