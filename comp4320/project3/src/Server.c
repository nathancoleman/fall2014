/*
*	This is the UDP Server for COMP4320 project3
*
*	Based from: 
*	http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/udpserver.c
*
*	Author: Lucas Saltz
*	Usage: ./ServerUDP <port>

//10010 + 5*20 
SERVERPORT 10110
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
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int rangeCheck (short portno, int gid) 
{
    int lowerRange = 10010 + (gid * 5);
    int upperRange = lowerRange + 4;

    if (portno >= lowerRange && portno <= upperRange)
        return TRUE;
    else
        return FALSE;
}


int main(int argc, char **argv)
{
    int sockfd; /* socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr, waitingaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
    int clientPo = 0; /* port the client wants to play on */
    int waitingClient = FALSE; /* is there a client waiting to play? */


    /* 
    * check command line arguments 
    */
    if (argc != 2)
    { //change this to 5???
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);
    printf("Waiting for client to connect\n\n");
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
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
        error("ERROR on binding");

    /* 
    * main loop: wait for a datagram, then echo it
    */
    clientlen = sizeof(clientaddr);
    while (1) 
    {

        /*
        * recvfrom: receive a UDP datagram from a client
        */
        bzero(buf, BUFSIZE);
        n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
        if (n < 0)
            error("ERROR in recvfrom");


        /*******************************************************
        Starting new code not from example code
        ********************************************************/

        int gid = buf[2];
        short portNo = (short)((buf[3] << 8) + buf[4]);  // getting client port number
        //printf("\tClient Port: %d\n", portNo);
        printf("Client connected\n");
        int validRange = rangeCheck(portNo, gid);
        //IF VALID
        if (validRange)  //valid
        {
            //IF WAITING CLIENT
            if (waitingClient) 
            {
                printf("Client waiting, sending address and port...\n");
                printf("\tIP Address: %s\n", hostaddrp);
                printf("\tPort: %d\n", clientPo);
                waitingClient = FALSE;
                unsigned char message[9];//Size of 9???
                message[0] = 0x12;
                message[1] = 0x34;
                message[2] = 20;
                message[3] = (waitingaddr.sin_addr.s_addr&0xFF);
                message[4] = ((waitingaddr.sin_addr.s_addr&0xFF00)>>8);
                message[5] = ((waitingaddr.sin_addr.s_addr&0xFF0000)>>16);
                message[6] = ((waitingaddr.sin_addr.s_addr&0xFF000000)>>24);
                message[7] = clientPo >> 8;
                message[8] = clientPo & 0xff;//portno of waiting client 5 - messed up

                n = sendto(sockfd, message, 9, 0, 
                (struct sockaddr *) &clientaddr, clientlen);
                
                if (n < 0) 
                    error("ERROR sending response");
                
                printf("Sending response...\n\n");
            }
            else
            {
                hostaddrp = inet_ntoa(clientaddr.sin_addr);
                waitingaddr = clientaddr;


                if (hostaddrp == NULL)
                    error("ERROR on inet_ntoa\n");


                //No waiting client, so this is the first set up, so next time there will be waiting client
                printf("No waiting client, storing address and port...\n");
                waitingClient = TRUE;
                clientPo = (((unsigned int)(unsigned char)buf[3] << 8) + (unsigned int)(unsigned char)buf[4]); // most sig of port and least sig of byte of port
                hostaddrp = inet_ntoa(clientaddr.sin_addr);
                printf("\tIP Address: %s\n", hostaddrp);
                printf("\tPort: %d\n", clientPo);
                unsigned char message[5];
                message[0] = 0x12;
                message[1] = 0x34;
                message[2] = 20; //gid
                message[3] = buf[3]; //Ph - portno??
                message[4] = buf[4]; //Pl - portno???

                n = sendto(sockfd, buf, n, 0, 
                (struct sockaddr *) &clientaddr, clientlen);
                
                if (n < 0) 
                    error("ERROR sending response");

                printf("Sending response...\n\n");
            }
        }

        else 
        {   // Not Valid
            unsigned char message[5];
            message[0] = 0x12;
            message[1] = 0x34;
            message[2] = 20;
            message[3] = 0;
            message[4] = 1; //XY???
                // Range is 1-7 
            //NO magic number - XY0 = 1
            //Magic number - XY1 = 
            //Port out of range?? XY2 = 1
            printf("Invalid request - sending error response\n");
            n = sendto(sockfd, message, 5, 0, (struct sockaddr *) &clientaddr, clientlen);

        }

    }

}

