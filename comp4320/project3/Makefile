CC = gcc

all: pre-build Server

ServerUDP: pre-build ServerUDP.o
	@$(CC) -o bin/Server obj/Server.o
ServerUDP.o: src/Server.c
	@$(CC) -c src/Server.c -o obj/Server.o


pre-build:
	@mkdir -p bin obj

tar: clean
	@tar -zcf ./lab320.tar.gz *

zip: clean
	@zip -r ./lab320 ./

clean:
	@rm -rf bin obj *.tar.gz *.zip