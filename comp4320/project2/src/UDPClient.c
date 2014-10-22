/*
*	This is the UDP Client for COMP4320 project2
*
*	Based from: 
*	
*
*	Author: Lucas Saltz
*	Usage:  
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0


//unsigned char checkSum(unsigned char *message, ) {}

//unsigned char checkOfCheckSum(unsigned char *message, ) {}


int main(int argc, char **argv) {
	int sockfd, portno, n;
	unsigned int length = 6;
	unsigned char gid = 16;
	unsigned char reqID;
	unsigned char delim = "~";
	struct sockaddr_in serveraddr;
	struct hostent *server;
	int clientLen;
	char *hostname;
	char *pNames[argc-4];
	unsigned char buf[BUFSIZE];

	if (!(argc >= 5)) 
	{
		fprintf(stderr, "\n", "bad arguments"); //expand this error message 
		exit(1);
	}

	hostname = argv[1];
	portno = atoi(argv[2]);
	reqID = atoi(argv[3]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//do error checks here

	server = gethostbyname(hostname);
	//error checks here

	


	
return 0;	
}