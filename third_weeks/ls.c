#include <stdio.h>
#include<getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
//#include "linkedlist.h"

#define MAX_SIZE 1024

int show_l(char *);

int main(int argc,char **argv)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	char current_path[MAX_SIZE];
	int oc = 0;
	int option_index = 0;
	int i = 0,j = 0;
	char file_name[MAX_SIZE];
	char *temp = NULL;
	int flag_a = 0,flag_l = 0;
	static struct option long_options[] = 
	{
	};

	while( (oc = getopt_long(argc,argv,"a::l::",long_options,&option_index)) != -1)
	{
		switch(oc)
		{
			case 'a':
				flag_a = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
		}
	}
	if(optind !=argc)
	{
		for(i = optind;i < argc ;i++)
		{
			if((dp = opendir(argv[i])) == NULL)
			{
				printf("can't find this file\n");
				break;
			}
			while((dirp = readdir(dp)) != NULL)
			{
				strcpy(file_name,argv[i]);
				temp = strcat(file_name,"/");
				temp = strcat(file_name,dirp->d_name);
				if(flag_a == 0)
				{
					if(dirp->d_name[0] != '.')
					{
						show_l(temp);
						printf("   %-10s\n",dirp->d_name);
					}
				}
				else
				{
					show_l(temp);
					printf("   %-10s\n",dirp->d_name);
				}
			}
			closedir(dp);
		}
	}
	else
	{
		if((dp = opendir(".")) == NULL)
		{
			printf("can't find this file\n");
			return 0;
		}
		while((dirp = readdir(dp)) != NULL)
		{
			if(flag_a == 0)
			{
				if(dirp->d_name[0] != '.')
				{
					show_l(dirp->d_name);
					printf("   %-10s\n",dirp->d_name);
				}
			}
			else
			{
				show_l(dirp->d_name);
				printf("   %-10s\n",dirp->d_name);
			}
		}
		closedir(dp);
	}
	return 0;
}
int show_l(char * file_name)
{
	struct stat buf;
	struct passwd *pw = NULL;
	struct group *gr = NULL;
	int j = 0;
	int permission[9]=
	{
		S_IRUSR,S_IWUSR,S_IXUSR,

		S_IRGRP,S_IWGRP,S_IXGRP,

		S_IROTH,S_IWOTH,S_IXOTH
	};

	stat(file_name,&buf);
	switch(buf.st_mode & S_IFMT)
	{
		case S_IFREG:
			printf("-");
			break;
		case S_IFDIR:
			printf("d");
			break;
		case S_IFLNK:
			printf("l");
			break;
		case S_IFBLK:
			printf("b");
			break;
		case S_IFCHR:
			printf("c");
			break;
		case S_IFIFO:
			printf("p");
			break;
		case S_IFSOCK:
			printf("s");
			break;
		default :
			break;
	}
	for(j = 8;j >= 0;j--)
	{
		if(permission[8-j] == (buf.st_mode & (1 << j)))
		{
			switch(j%3)
			{
				case 2:
					printf("r");
					break;
				case 1:
					printf("w");
					break;
				case 0:
					printf("x");
					break;
				default :
					break;
			}
		}
		else
		{
			printf("-");
		}
	}

	pw = getpwuid(buf.st_uid);
	gr=getgrgid(buf.st_gid);
	printf("  %2d %s  %s  %4ld",(int)buf.st_nlink,pw->pw_name,gr->gr_name,buf.st_size);
	return 0;
}
