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
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

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

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char** argv){
	
	/* First is bin/clientTCP, then we need hostname, port, op, and message */
	if(argc != 5)
	{
		printf("Usage: bin/clientTCP host port message\n");
		exit(1);
	}

	char *host = argv[1];
	char *port = argv[2];
	int op = atoi(argv[3]);
	char *message = argv[4];

	int clientSocket;
	char buffer[1029];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	int sockfd, numbytes;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

	packed_out *pd = malloc(sizeof(packed_out));
	pd->message_length = htons(5 + strlen(message));
	pd->request_id = htons(request_id);
	pd->operation = op;
	memcpy(pd->message, message, strlen(message));

	gettimeofday(&request, NULL);

	send(sockfd, pd, pd->message_length, 0);

	packed_in *pi = malloc(sizeof(packed_in));

	recv(sockfd, pi, 1029, 0);

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

	printf("Request ID: %d\n", ntohs(pi->request_id));
	printf("Roundtrip time: %d microseconds\n", dt);

	return 0;
}