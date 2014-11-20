/*
*	This is the UDP Server for COMP4320 project3
*
*	Based from: 
*	http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*
*	Author: Lucas Saltz
*	Usage: ./ServerUDP <port>



0x1234GIDPhPl
number 0x1234 (over two bytes), your group ID GID (one byte) (GID is the number of the 
group who designed/implemented the client.), and a port number P (PhPl
significant byte of the port, and Pl
whether there is a waiting client or not:
i. If there is no waiting client, the server registers the IP address of NewClient and the port 
number P, and responds with a datagram that contains 0x1234GIDPhPl
is the magic number, GID is the GID of the owner of the server, and PhPl
ii. If there is a waiting client WaitingClient, the server responds with a message that 
1.The magic number 0x1234
2.The GID where GID is the group ID of the group implementing the server.
3.The IP address of WaitingClient (network byte order)
4.The port number of WaitingClient. (network byte order)
 from client NewClient that contains the magic 
 is the least significant byte of the port), it reacts depending on
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
//10010 + 5*20 
#define SERVERPORT 10100

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
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
  printf("Waiting for client to connect\n");
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




		n = sendto(sockfd, buf, BUFSIZE, 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
		printf("sent response back\n");
    	if (n < 0) 
      		error("ERROR in sendto");
  	}


  		n = sendto(sockfd, buf, n, 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
		printf("sent back to client\n");
    	if (n < 0) 
      		error("ERROR in sendto");
   	
  

}

