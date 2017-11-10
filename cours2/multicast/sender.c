/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 * Christian Toinard, 20/11/2016 :
 * enables a message to send from the arguments 
 * sent once the input message,
 * improve the explanations
 * and should compile without warnings (at least on true Unix e.g. MacOS) !!
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HELLO_PORT  12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE  256


int main(int argc, const char *argv[])
{
	if(argc <= 1)
	{
		printf("Pas de paramètre renseigné.\nCe programme s\'utilise ainsi: `%s \"Message de test.\"`\n", argv[0]);
		exit(1);
	}

	char message[MSGBUFSIZE];
	printf("sizeMessageBuffer = %zu\n", sizeof(message));
	if((strlen(argv[1]) < sizeof(message)))
	{
		printf("sizeMessage = %zu\n", strlen(argv[1]));
		strcpy(message, argv[1]);
	}

	else
	{
		perror("argv[1] is too long !");
		exit(2);
	}

	/* Create what looks like an ordinary UDP socket */
	int nSD;
	if((nSD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(3);
	}

	/* CT: set up destination extremity */
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family      = AF_INET;
	server.sin_addr.s_addr = inet_addr(HELLO_GROUP);
	server.sin_port        = htons(HELLO_PORT);

	/* CT: now just sendto() once the input message to our destination ! */
	if(sendto(nSD, message, sizeof(message), 0, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("sendto()");
		exit(4);
	}

	else
	{
		printf("\nGood Sending of: \"%s\"\n", message);
	}

	return 0;
}
