/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *		the group to its stdout...
 *
 * Antony Courtney,	25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 * Modified by : Christian Toinard (20/11/2016) 
 * to enable reuse of port for any extremity as true BSD systems allow, 
 * improve compilation without warning, 
 * remove some bugs 
 * and update the explanations
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256


int main(int argc, const char *argv[])
{
	/* Create what looks like an ordinary UDP socket */
	int nSD;
	if((nSD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(1);
	}

	/* CT: allow multiple socket descriptors to use the same extremity  */
	socklen_t yes = 1;
	if(setsockopt(nSD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		perror("Reusing ADDR failed()");
		exit(2);
	}

	/* CT: Set up the local / server extremity */
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family      = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port        = htons(HELLO_PORT);

	/* CT: bind the descriptor to the listening local extremity */
	if(bind(nSD, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("bind()");
		exit(3);
	}

	/* Use setsockopt() to request that the kernel join a multicast group */
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt(nSD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt()");
		exit(4);
	}

	else
	{
		printf("Démarrage de l\'écoute de messages adressés au groupe %s:%u...\n\n", HELLO_GROUP, HELLO_PORT);
	}

	/* Now just enter a read-print loop */
	char msgbuf[MSGBUFSIZE];
	struct sockaddr_in client;
	ssize_t nbytes;

	while(1)
	{
		socklen_t addrlen = sizeof(client);
		if((nbytes = recvfrom(nSD, msgbuf, MSGBUFSIZE, 0, (struct sockaddr*) &client, &addrlen)) < 0)
		{
			perror("recvfrom()");
			exit(5);
		}

		msgbuf[nbytes] = '\0';
		printf("Message reçu: \"%s\"\n", msgbuf);
	}
}
