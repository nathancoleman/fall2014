/*
*	This is the UDP Client for COMP4320 project2
*
*	Based from: 
*   http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#datagram
*	http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*	
*	Compile using gcc (not g++)
*

UDPClient Servername Port# requestID n1 n2 ..nm where 
1.	UDPClient is your executable, 
2.	Servername is the server name,
3.	 port# is a port number,  
4.	RequestID  is a request ID from 0 to 127, and 
5.	ni  is a host name.

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


unsigned char checkSum(unsigned char *message, size_t nBytes) {

	unsigned short sum = 0;

	int i;
	for (i = 0; i < nBytes; i++)
	{
		sum += *(message++);
		sum = (unsigned short)((sum & 0xFF) + (sum >> 8));
		sum = (unsigned short)((sum & 0x00ff));
	}
	unsigned char check  = (unsigned char)(~sum);
	return check;
}

unsigned char checkTheCheckSum(unsigned char *message, size_t nBytes) {

	unsigned short sum = 0;

	int i;
	for (i = 0; i < nBytes; i++)
	{
		sum += *(message++);
		sum = (unsigned short)((sum & 0xFF) + (sum >> 8));
		sum = (unsigned short)((sum & 0x00ff));
	}
	unsigned char check  = (unsigned char)(sum);
	return check;
}

void error(char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char **argv) {
	int sockfd, n, portno;
	unsigned int length = 6;
	unsigned char gid = 16; //GID: GID is the group ID of the group who sends the request. It is stored on one byte (Byte3).
	unsigned char delim = '~';
	unsigned char reqID;
	struct sockaddr_in servaddr;
	struct hostent *server;
	int clientLen;
	char *hostname;
	char *pNames[argc-4];
	unsigned char buf[BUFSIZE];
	
	unsigned short rLength;

	printf("Starting client\n");

	if (!(argc >= 5)) 
	{
		fprintf(stderr, "bad arguments\n"); //expand this error message 
		exit(1);
	}

	printf("Passed initial argc checks\n");

	hostname = argv[1];
	portno = atoi(argv[2]);
	reqID = atoi(argv[3]);
	if(0 < reqID || reqID > 127)
		printf("Error: request ID not valid - Continuing anyway...\n");

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//do error checks here
	if (sockfd < 0) 
    	error("ERROR opening socket");


	server = gethostbyname(hostname);
	//error checks here
	if (server ==  NULL)
	{
		fprintf(stderr, "ERROR, not host by name %s\n", hostname);
		exit(1);
	}

	printf("Obtained socket and server\n");

	/*
     * recvfrom: receive a UDP datagram from a client
     */
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
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);


    /*
	The port number (the transport address). 
	You can explicitly assign a transport address (port) or allow the operating system to assign one. 
	If you're a client and don't need a well-known port that others can use to locate you (since they will only respond to your messages), 
	you can just let the operating system pick any available port number by specifying port 0. 
	If you're a server, you'll generally pick a specific number since clients will need to know a port number to which to address messages.
    */
     servaddr.sin_port=htons(portno); 
     //htons : host to network - short : convert a number into a 16-bit network representation. This is commonly used to store a port number into a sockaddr structure.

     //Building message from CM
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
     memcpy(&sendBuffer[4], &reqID, 1);
     memcpy(&sendBuffer[5], &delim, 1);
     int position = 6;
     int count = 0;
     int isValid = FALSE;


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
    sendBuffer[2] = checkSum(sendBuffer, length);
    bzero(buf, BUFSIZE);

    int trys;
        //printf("Sending messages\n");
    for (trys = 0; trys < 7; trys++) 
    {
    	printf("Sending Message, try number#: %d\n", trys);
    	clientLen = sizeof(servaddr);
    	n = sendto(sockfd, sendBuffer, length, 0, (struct sockaddr*)&servaddr, clientLen);
    	if (n < 0)
    	{
    		//Must be something there to send
    		error("Error in sendto");
    	}
    	n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr*)&servaddr, &clientLen);
    	if (n < 0) 
    	{
    		error("Error in recvfrom");
    	}
    	unsigned char check  = checkTheCheckSum(buf, sizeof(buf));
		//printf("check = %d\n", check);     
		if (check == 255) 
	    	{
	    		short incLength = (unsigned short)((buf[0]<<8) + buf[1]);
	    		//printf("incLength: %d\n", incLength);
	    		//printf("%s\n", n);

	    		if (n == 7) // can only try 7 times
	    		{
	    			printf("error has occured from server side");
	    			break;
	    		}
	    		int i = 5;
	    		int e = 0;
	    		for (e; e < count; e++)
	    		{
	    			printf("Host %s has IP address: ", pNames[e]);
	    			if(buf[i] == 255)
	    			{
	    				printf("Issues with host name/IP address\n");
	    				i +=4;
	    			}
	    			printf("%d.", buf[i++]);
	    			printf("%d.", buf[i++]);
	    			printf("%d.", buf[i++]);
	    			printf("%d\n", buf[i++]);
	    		}
	    		isValid = TRUE;
	    	}
    	else 
    		printf("Invalid response from server");
    	if (isValid == TRUE)
    		break;
    }
    return 0;
}
