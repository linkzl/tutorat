#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define LTEMPS 32

/*
	Prend l'habitude d'utiliser l'option -Wall quand tu compile
	et vérifie qu'il n'y ai aucune erreurs.

	rajoute des comentaire dans ton code pour expliquer ce 
	que fais chaques fonctions ainsi que les arguments.


	l'affichage "classique" de ls est plutôt :
	droits user groupe taille date nom
	toi tu as mit les user et groupe avant les droits


*/



int main(int argc, char const *argv[])
{
	//struct stat sstat;// non utilisée pour le moment
	struct stat info;
	char type;
	char pws[9],grs[9],temps[LTEMPS];
	if (lstat(argv[1],&info)!=1)// lstat prend en compte les link
	{
		struct passwd *pw=getpwuid(info.st_uid);
		if (pw != NULL)
		{
			//printf("%s \t",pw->pw_name );
			sprintf(pws,"%8d",(int)info.st_uid);

		}
		else {
			printf("%lu \t",(unsigned long int)info.st_uid);// cast pour passer en lu
		}
		struct group *gr=getgrgid(info.st_gid);
		if (gr!=NULL)
		{
			//printf("%s\t",gr->gr_name );
			sprintf(grs,"%8d",(char)info.st_gid);
		}
		else
		{
			printf("%lu\t",(unsigned long int)info.st_gid );// idem
		}
		if (S_ISREG(info.st_mode))
		{
			type='-';
		}
		else if (S_ISDIR(info.st_mode))
		{
			type='d';
		}
		else if (S_ISLNK(info.st_mode))//pour prendre en compte les liens symboliques.
		{							   //
			type='l';				   //
		}							   //
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
		printf("%c%c%c%c%c%c%c%c%c%c %2d %s %s %9d %s",type,
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
				pw->pw_name, // modification affichage user
				gr->gr_name, // modification affichage group
				(int)info.st_size,
				temps);
//il manquais l'affichage du nom du fichier !
		printf(" %s\n",argv[1] );
	}
	
//rajoute des commentaires ton fichier pour expliquer ce que tu fais


/*	if (argc < 2)		
	{
		if (fstat(STDIN_FILENO,&sstat)==-1)
				{
					fprintf(stderr, "%s: impossible d'obtenir le statut de %s \n",argv[0],"<STDIN>" );
					exit(EXIT_FAILURE);
				}
		printstat("<STDIN>\n",&sstat);		
	}
	else 
	{
		int i;
		for (i = 0; i < argc; ++i)
		{
			if (stat(argv[i],&sstat)==-1)
			{
				perror("problem with the stat fonction");
				exit(EXIT_FAILURE);
			}
		printstat(argv[i],&sstat);
		}
	}*/
	exit(EXIT_FAILURE);
}