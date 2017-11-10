/*
1.a.1
SIOCGIFCONF
*/
/*
1.a.2
bcopy(&ifr->ifr_broadaddr, &dst, sizeof(ifr->ifr_broadaddr));
dst.sin_family = AF_INET;
dst.sin_port   = htons(5011);
*/


#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ioctl.h>

#define BUFFERSIZE 1024

typedef void (*sighandler_t)(int);


void catch(void)
{
	exit(EXIT_SUCCESS);
}


int main(int argc, const char *argv[])
{
	signal(SIGINT,(sighandler_t)catch);

	int sdemande;
	if((sdemande = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	int on = 1;
	if(setsockopt(sdemande, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
	{
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

	char liste[BUFFERSIZE];

	struct ifconf ifc;
	ifc.ifc_len = sizeof(liste);
	ifc.ifc_buf = liste;

	if(ioctl(sdemande, SIOCGIFCONF, (char*)&ifc) < 0)
	{
	    perror("ioctl SIOCGIFCONF");
	    exit(EXIT_FAILURE);
	}

	struct ifreq *ifr = ifc.ifc_req;

	int n = ifc.ifc_len / sizeof(*ifr);
	for(; --n >= 0; ifr++)
	{
		//printf("Interface %s:\n", ifr->ifr_name);

		if(ioctl(sdemande, SIOCGIFFLAGS, (char*)ifr) < 0)
		{
			perror("ioctl() (IFFLAG)");
			exit(EXIT_FAILURE);
		}

		if(((ifr->ifr_flags & IFF_UP) == 0) || (ifr->ifr_flags & IFF_LOOPBACK) || (ifr->ifr_flags & IFF_POINTOPOINT) || ((ifr->ifr_flags & IFF_BROADCAST) == 0))
		{
			continue;
		}

		if(ioctl(sdemande, SIOCGIFBRDADDR, (char*)ifr))
		{
			perror("ioctl() (BROADADDR)");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in dst;
		bcopy(&ifr->ifr_broadaddr, &dst, sizeof(ifr->ifr_broadaddr));
		dst.sin_family = AF_INET;
		dst.sin_port   = htons(5011);
		sendto(sdemande, "Bonjour c\'est louis", 23, 0, (struct sockaddr*)&dst, sizeof(dst));
		
	}
	return 0;
}
