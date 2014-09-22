/*
*	This is the TCP Client for COMP4320 project1
*	Author: Lucas Saltz/Nathan Coleman
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
	
	/* First is bin/clientTCP, then we need hostname, port, and message */
	if(argc != 4)
	{
		printf("Usage: bin/clientTCP host port message");
		exit(1);
	}

	char *host = argv[1];
	int port = atoi(argv[2]);
	char *message = argv[3];

	int clientSocket;
	char buffer[1029];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(host);
	addr_size = sizeof serverAddr;
	
	if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0)
		printf("ERROR CONNECTING\n");
	else
		printf("CONNECTED\n");

	sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	return 0;
}