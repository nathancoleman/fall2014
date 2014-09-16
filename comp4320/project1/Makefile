CC = gcc

all: pre-build serverUDP clientTCP

serverUDP: pre-build serverUDP.o
	@$(CC) -o bin/serverUDP obj/serverUDP.o
serverUDP.o: src/serverUDP.c
	@$(CC) -c src/serverUDP.c -o obj/serverUDP.o

clientTCP: pre-build clientTCP.o
	@$(CC) -o bin/clientTCP obj/clientTCP.o
clientTCP.o: src/clientTCP.c
	@$(CC) -c src/clientTCP.c -o obj/clientTCP.o

pre-build:
	@mkdir -p bin obj

clean:
	@rm -rf bin obj