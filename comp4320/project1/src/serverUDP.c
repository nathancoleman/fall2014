/*
*	This is the UDP Server for COMP4320 project1
*
*	Based from: 
*	http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*
*	Author: Lucas Saltz
*
*/

/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
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

#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

/*
This function counts the number of vowels in the buffer
*/
short vowelNumber(char* wordBuffer, size_t size) {
	short vowelCount = 0;
	unsigned int i;

	for (i = 0; i < size; i++) 
	{
		switch(wordBuffer[i]) {
			case 'a':
			case 'A':
			case 'e':
			case 'E':
			case 'i':
			case 'I':
			case 'o':
			case 'O':
			case 'u':
			case 'U':
					vowelCount += 1;
					break;
		}
	}
	return vowelCount;
}

int isaVowel(char a) 
{
	switch(a)
	{
		case 'a':
		case 'A':
		case 'e':
		case 'E':
		case 'i':
		case 'I':
		case 'o':
		case 'O':
		case 'u':
		case 'U':
			return TRUE;
	}
	return FALSE;
}

char* disemVoweling(char* wordBuffer, size_t size) 
{
	char* p = malloc(size - vowelNumber(wordBuffer, size)); //i +size??
	unsigned int i, j=0;

	for(i=0; i < size; i++)
	{
		if (isaVowel(wordBuffer[i]) == FALSE) //removing char if not a vowel
		{
			p[j] = wordBuffer[i];
			j += 1;
		}
	}
	p[j] = '\0'; //taking off last part of the array ??	
	return p;
}


int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  if (argc != 2) { //change this to 5???
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    /* 
     * gethostbyaddr: determine who sent the datagram
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");

  	
  	/*******************************************************
		Starting new code not from example code
  	********************************************************/
  	char operation = buf[4];
  	//print out the operation
  	printf("Operation recieved: ");
  	printf("%x\n", operation);


  	if(operation == 0x55) //0x55 is hex for vLength
  	{
  		printf("%s\n", "operation == 0x55");
  		short packetLength = (short)((buf[0] << 8) + buf[1]);
  		char buffer[packetLength-5]; //-5 for known size
  		int i;
  		for (i = 0; i < sizeof(buffer); i++)
  		{
  			buffer[i] = buf[i+5];
  		}
  		short numVowels = vowelNumber(buffer, sizeof(buffer));
  		char response[6];
  		short length = (short) sizeof(response);
  	/*******************************************************
		implement response message to send back below
  	********************************************************/
		response[0] = (char)(length >> 8);  //shifting the bits
		response[1] = (char)(length & 0xff); //taking just the end
		response[2] = buf[2];
		response[3] = buf[3];
		response[4] = (char)(numVowels >> 8);
		response[5] = (char)(numVowels & 0xff);
		n = sendto(sockfd, response, length, 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    	if (n < 0) 
      		error("ERROR in sendto");
  	}
  	else if (operation == -86) //hack
  	{
  		printf("%s\n", "operation == 0xAA");
  		short packetLength = (short)((buf[0] << 8) + buf[1]);
  		char buffer[packetLength-5];
  		int i;
  		for (i = 0; i < sizeof(buffer); i++) //i++???
  		{
  			buffer[i] = buf[i+5];
  		}
  		char* stringV = disemVoweling(buffer, sizeof(buffer));
  		short length = (short)(strlen(stringV) + 4);
  		char byteMessage[length];
  		byteMessage[0] = (char)(length >> 8);
  		byteMessage[1] = (char)(length & 0xff);
  		byteMessage[2] = buf[2];
  		byteMessage[3] = buf[3];

  		for (i = 0; i < (length - 4); i++) //i++???
  		{
  			byteMessage[i+4] = stringV[i];
  		}
  		n = sendto(sockfd, byteMessage, length, 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    	if (n < 0) 
      		error("ERROR in sendto");
   	}
  

  }
}

