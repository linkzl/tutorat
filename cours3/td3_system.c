#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LTEMPS 32
int main(int argc, char const *argv[])
{
	struct stat sstat;
	struct stat info;
	char type;
	char pws[9],grs[9],temps[LTEMPS];
	if (stat(argv[1],&info)!=1)
	{
		struct passwd *pw=getpwuid(info.st_uid);
		if (pw != NULL)
		{
			printf("%s \n",pw->pw_name );
			sprintf(pws,"%8d",(int)info.st_uid);

		}
		else {
			printf("%li \n",info.st_uid);
		}
		struct group *gr=getgrgid(info.st_gid);
		if (gr!=NULL)
		{
			printf("%s\n",gr->gr_name );
			sprintf(grs,"%8d",(char)info.st_gid);
		}
		else
		{
			printf("%s\n",info.st_gid );
		};
		if (S_ISREG(info.st_mode))
		{
			type='-';
		}
		else if (S_ISDIR(info.st_mode))
		{
			type='d';
		}
		else if (S_ISCHR(info.st_mode))
		{
			type='c';
		}
		else if (S_ISBLK(info.st_mode))
		{
			type='b';
		}
		else if (S_ISFIFO(info.st_mode))
		{
			type='p';
		}
		strftime(temps,LTEMPS,"%a %e %h %Y %H:%M:%S",localtime(&(info.st_mtime)));
		printf("%c%c%c%c%c%c%c%c%c%c %2d %8s %8s %9d %s \n",type,
				info.st_mode&S_IRUSR?'r':'-',
				info.st_mode&S_IWUSR?'w':'-',
				info.st_mode&S_IXUSR?'x':'-',
				info.st_mode&S_IRGRP?'r':'-',
				info.st_mode&S_IWGRP?'w':'-',
				info.st_mode&S_IXGRP?'x':'-',
				info.st_mode&S_IROTH?'r':'-',
				info.st_mode&S_IWOTH?'w':'-',
				info.st_mode&S_IXOTH?'x':'-',
				(int)info.st_nlink,
				pws,
				grs,
				(int)info.st_size,
				temps);

	}
	/*int i;
	if (argc < 2)		
	{
		if (fstat(STDIN_FILENO,&sstat)==-1)
				{
					fprintf(stderr, "%s: impossible d'obtenir le statut de %s \n",argv[0],"<STDIN>" );
					exit(EXIT_FAILURE);
				}
		print_stat("<STDIN>\n",&sstat);		
	}
	else {
		int i;
		for (i = 0; i < argc; ++i)
		{
			if (stat(argv[i],&sstat)==-1)
			{
				perror("problem with the stat fonction");
				exit(EXIT_FAILURE);
			}
		print_stat(argv[i],&sstat);
		}
	}
	exit(EXIT_FAILURE);*/
}