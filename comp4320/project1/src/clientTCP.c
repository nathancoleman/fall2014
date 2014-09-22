/*
*	This is the TCP Client for COMP4320 project1
*	Author: Lucas Saltz/Nathan Coleman
*/

#include <stdio.h>
#include <sys/socket.h>

int main(int argc, char**argv)
{
	// We should get 3 args w/ argv=[bin/clientTCP, ip, port]
	// Usage: bin/clientTCP localhost 8001 (i.e.)
	if (argc != 3)
	{
      	printf("ERROR: Expected IP and Port");
      	exit(1);
   	}

	return 0;
}