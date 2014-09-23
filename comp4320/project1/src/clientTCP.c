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
#include <sys/time.h>

const int GETVLENGTH = 0x55;
const int DISEMVOWEL = 0xAA;
char request_id = 1;

struct packed_outgoing
{
	uint16_t message_length;
	uint16_t request_id;
	uint8_t operation;
	char message[1024]; // Message not more than 1k
} __attribute__((__packed__));

struct packed_incoming
{
	uint16_t message_length;
	uint16_t request_id;
	char message[1024]; // Message not mroe than 1k
}__attribute__((__packed__));

struct timeval request, response;

typedef struct packed_outgoing packed_out;
typedef struct packed_incoming packed_in;

int main(int argc, char** argv){
	
	/* First is bin/clientTCP, then we need hostname, port, op, and message */
	if(argc != 5)
	{
		printf("Usage: bin/clientTCP host port message\n");
		exit(1);
	}

	char *host = argv[1];
	int port = atoi(argv[2]);
	int op = atoi(argv[3]);
	char *message = argv[4];

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
	{
		printf("ERROR CONNECTING\n");
		exit(1);
	}
	else
		printf("CONNECTED\n");

	packed_out *pd = malloc(sizeof(packed_out));
	pd->message_length = htons(5 + strlen(message));
	pd->request_id = htons(request_id);
	pd->operation = op;
	memcpy(pd->message, message, strlen(message));

	gettimeofday(&request, NULL);

	send(clientSocket, pd, pd->message_length, 0);

	packed_in *pi = malloc(sizeof(packed_in));

	recv(clientSocket, pi, 1029, 0);

	gettimeofday(&response, NULL);

	suseconds_t dt = (response.tv_usec - request.tv_usec);

	if(pd->operation == GETVLENGTH)
	{
		uint16_t vowel_count = pi->message[0] << 8;
		vowel_count = pi->message[1] | vowel_count;
		printf("Vowel count received: %d\n", vowel_count);
	}
	else if(pd->operation == DISEMVOWEL)
	{
		printf("Disemvoweled message: %s\n", pi->message);
	}

	printf("Roundtrip time: %d microseconds\n", dt);

	return 0;
}