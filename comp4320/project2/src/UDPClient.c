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
	unsigned char delim = '~';
	struct sockaddr_in servaddr;
	struct hostent *server;
	int clientLen;
	char *hostname;
	char *pNames[argc-4];
	unsigned char buf[BUFSIZE];
	
	unsigned short rLength;

	if (!(argc >= 5)) 
	{
		fprintf(stderr, "bad arguments\n"); //expand this error message 
		exit(1);
	}

	hostname = argv[1];
	portno = atoi(argv[2]);
	reqID = atoi(argv[3]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//do error checks here

	server = gethostbyname(hostname);
	//error checks here



	bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET; //The address family we used when we set up the socket. In our case, it's AF_INET.
    /*
		The address for this socket. This is just your machine's IP address. 
		With IP, your machine will have one IP address for each network interface. 
		For example, if your machine has both Wi-Fi and ethernet connections, 
		that machine will have two addresses, one for each interface. 
		Most of the time, we don't care to specify a specific interface and can let the operating system use whatever it wants. 
		The special address for this is 0.0.0.0, defined by the symbolic constant INADDR_ANY.
    */

 //   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    

    /*
	The port number (the transport address). 
	You can explicitly assign a transport address (port) or allow the operating system to assign one. 
	If you're a client and don't need a well-known port that others can use to locate you (since they will only respond to your messages), 
	you can just let the operating system pick any available port number by specifying port 0. 
	If you're a server, you'll generally pick a specific number since clients will need to know a port number to which to address messages.
    */
     servaddr.sin_port=htons(portno); 
     //htons : host to network - short : convert a number into a 16-bit network representation. This is commonly used to store a port number into a sockaddr structure.


     for (n = 4; n < argc; n++)
     {
     	if ((n+1) == argc) 
     	{
     		length += strlen(argv[n]);
     	}
     	else
     	{
     		length += strlen(argv[n]) + 1;
     	}
     }
     rLength = htons(length);
     /*
		void * memcpy ( void * destination, const void * source, size_t num );
		Copy block of memory
		Copies the values of num bytes from the location pointed by source directly to the memory block pointed by destination.
     */
	unsigned char sendBuffer[length];
     memcpy(&sendBuffer[0], (char*)&rLength, 2);
     sendBuffer[2] = 0;
     memcpy(&sendBuffer[3], &gid, 1);
     memcpy(&sendBuffer[4], &gid, 1);
     memcpy(&sendBuffer[5], &gid, 1);
     int position = 6;
     int count = 0;


     for (n = 4; n < argc; n++)
     {
     	if ((n + 1) == argc)
     	{
     		pNames[count++] = argv[n];
     		sprintf(&sendBuffer[position], "%s", argv[n]);
     	}
     	else
     	{
     		pNames[count++] = argv[n];
     		sprintf(&sendBuffer[position], "%s", argv[n]);
     		position += strlen(argv[n]);
     		sendBuffer[position++] = delim;
     	}
     }


    //pack the message

    // send the message



	
return 0;	
}