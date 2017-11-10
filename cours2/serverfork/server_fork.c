/*
2.a.1
case 0:
				close(sockfd);
				printf("Fils [%d]\n", getpid());

				while(1)
				{
					int nbs = send(clientfd, buffer, (sizereceveid = recv(clientfd, buffer, MAXBUF, 0)), 0);
					printf("[%d] : \n", getpid());

					write(1, buffer, sizereceveid);

					if(nbs == 0)
					{
						close(clientfd);
						printf("Fin [%d]: \n", getpid());
						exit(1);
					}

					if(strcmp(buffer, "/exit\r\n") == 0)
					{
						close(clientfd);
						exit(0);
					}
				}

			default:
				nbclient++;
				close(clientfd);
				break;
*/


#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MY_PORT 12345
#define MAXBUF	1024


int main(int argc, const char *argv[])
{
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(errno);
	}

	struct sockaddr_in self;
	bzero(&self, sizeof(self));
	self.sin_family      = AF_INET;
	self.sin_port        = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0)
	{
		perror("bind()");
		exit(errno);
	}

	if(listen(sockfd, 20) != 0)
	{
		perror("listen()");
		exit(errno);
	}

	printf("Père [%d]\n", getpid());

	char buffer[MAXBUF];
	pid_t client[5];
	int nbclient = 0;
	ssize_t sizereceveid;

	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		int clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		switch((client[nbclient] = fork()))
		{
			case -1:
				perror("fork()");
				exit(EXIT_FAILURE);

			case 0:
				close(sockfd);
				printf("Fils [%d]\n", getpid());

				while(1)
				{
					int nbs = send(clientfd, buffer, (sizereceveid = recv(clientfd, buffer, MAXBUF, 0)), 0);
					printf("[%d] : \n", getpid());

					write(1, buffer, sizereceveid);

					if(nbs == 0)
					{
						close(clientfd);
						printf("Fin [%d]: \n", getpid());
						exit(1);
					}

					if(strcmp(buffer, "/exit\r\n") == 0)
					{
						close(clientfd);
						exit(0);
					}
				}

			default:
				nbclient++;
				close(clientfd);
				break;
		}
	}

	close(sockfd);

	return 0;
}
