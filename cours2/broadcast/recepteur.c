#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFERSIZE 256

typedef void (*sighandler_t)(int);


void catch(void)
{
	exit(EXIT_SUCCESS);
}


int main(int argc, const char *argv[])
{
	signal(SIGINT, (sighandler_t)catch);

	int sdemande;
	if((sdemande = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in nom_sdemande;
	nom_sdemande.sin_family      = AF_INET;
	nom_sdemande.sin_addr.s_addr = htonl(INADDR_ANY);
	nom_sdemande.sin_port        = htons(5011);

	if(bind(sdemande, (const struct sockaddr*)&nom_sdemande, sizeof(nom_sdemande)) < 0)
	{
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);

	char buffer[BUFFERSIZE];

	ssize_t nc = recvfrom(sdemande, buffer, BUFFERSIZE, 0, (struct sockaddr *)&from, &fromlen);
	if(nc <= 0)
	{
		perror("recvfrom()");
		exit(EXIT_FAILURE);
	}

	buffer[nc] = '\0';

	printf("Le serveur a reçu: %s\n... de la part de %s:%d\n", buffer, inet_ntoa(from.sin_addr),htons(from.sin_port));

	return 0;
}
