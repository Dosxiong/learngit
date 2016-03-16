#include <stdio.h>
#include<syslog.h>
#include<stdlib.h>
#include<getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include<sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include "linkedlist.h"

#define MAX_SIZE 1024

int add_file(N *head,char *file_path,char * file_name);
void show_l(N *head,int flag_l);
int name_length_compare(void *a,void *b);
int file_size_compare(void *a,void *b);
int file_ctime_compare(void *a,void *b);
int recursion(char * path,int flag_a,int flag_t,int flag_s,int flag_l,int flag_r);
void adapt_width_show(N *head);
int file_name_compare(void *a,void *b);

typedef struct file{
	char      file_path[MAX_SIZE];
	char      file_name[MAX_SIZE];
	char      user[MAX_SIZE];
	char      group[MAX_SIZE];
	char      file_type;
	struct tm tm;
	int       ctime;
	int       link;
	int       file_name_length;
	long      file_size;
	int       permission[3];
}file_info;

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
	int flag_a = 0,flag_l = 0,flag_s = 0,flag_t = 0,flag_r = 0;
	N *head = NULL;
	static struct option long_options[] = 
	{
	};

	while( (oc = getopt_long(argc,argv,"a::l::t::s::r::",long_options,&option_index)) != -1)
	{
		switch(oc)
		{
			case 'a':
				flag_a = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'r':
				flag_r = 1;
				break;
			case 's':
				flag_s = 1;
				break;
			case 't':
				flag_t = 1;
				break;
			default :
				break;
		}
	}
	if(optind !=argc)
	{
		for(i = optind;i < argc ;i++)
		{
			recursion(argv[i],flag_a,flag_t,flag_s,flag_l,flag_r);
		}
	}
	else
	{
		recursion(".",flag_a,flag_t,flag_s,flag_l,flag_r);
	}
	return 0;
}

void adapt_width_show(N *head)
{
	struct winsize size;
	int max_col_width = 0;
	int row = 1;
	N ** linkedlist_array = NULL;
	int count = 0;
	int col = 0;
	N * p = NULL;
	int i = 0,j = 0,k = 0;
	int is_success = 1;
	int sum_length = 0;
	file_info * temp = NULL;
	int *maxsize_array = NULL;

	ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
	count = linkedlist_count(head);
	linkedlist_array = (N **)malloc(sizeof(N * )*(count));	
	if(linkedlist_array != NULL)
	{
		for(p = head->next,i = 0;p !=NULL;p = p->next,i++)
		{
			linkedlist_array[i] = (N *)p;
		}
		while(1)
		{
			for(i = 0;i < count;i += row)
			{
				for(k = i;(k < (i+row)) && (k < count);k ++)
				{
					temp = ((file_info *)linkedlist_array[k]->datapointer);
					if(max_col_width < temp->file_name_length)
					{
						max_col_width = temp->file_name_length;
					}
				}
				sum_length += max_col_width+2;
				max_col_width = 0;
			}
			if(sum_length <= size.ws_col)
			{
				break;
			}
			row ++;
			sum_length = 0;
		}
		if(count%row == 0)
		{
			col = count/row;
		}
		else
		{
			col = count/row+1;
		}

		maxsize_array = (int *)malloc(sizeof(int)*col);
		for(i = 0,j = 0;i < count;i += row,j++)
		{
			for(k = i;(k < (i + row)) && k < count;k ++)
			{
				temp = ((file_info *)linkedlist_array[k]->datapointer);
				if(max_col_width < temp->file_name_length)
				{
					max_col_width = temp->file_name_length;
				}
			}
			maxsize_array[j] = max_col_width;
			max_col_width = 0;
		}
		for(i = 0;i < row ;i++)
		{
			for(j = i,k = 0;j < count;j += row,k++)
			{
				temp = ((file_info *)linkedlist_array[j]->datapointer);
				printf("%*s",-(maxsize_array[k]+2),temp->file_name);
			}
			printf("\n");
		}
	}
}

int recursion(char * path,int flag_a,int flag_t,int flag_s,int flag_l,int flag_r)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	N *head = NULL;
	N *p = NULL;
	char file_path[MAX_SIZE];
	char *temp = NULL;
	file_info *file = NULL;

	if((dp = opendir(path)) == NULL)
	{
		printf("can't find this file\n");
		return -1;
	}
	head = create();
	while((dirp = readdir(dp)) != NULL)
	{
		if(flag_a == 0)
		{
			if(dirp->d_name[0] != '.')
			{
				add_file(head,path,dirp->d_name);
			}
		}
		else
		{
			add_file(head,path,dirp->d_name);
		}
	}
	closedir(dp);
	if((flag_t*flag_s) == 1)
	{
		printf("-t and -s can only exist one\n");
		return -1;
	}
	if(flag_t == 1)
	{
		string_sort_xier(head,file_ctime_compare);
	}
	else if(flag_s == 1)
	{
		string_sort_xier(head,file_size_compare);
	}
	else
	{
		string_sort_xier(head,file_name_compare);
	}
	printf("%s\n",path);
	adapt_width_show(head);
	//show_l(head,flag_l);
	if(flag_r == 1)
	{
		for(p = head->next;p != NULL;p = p->next)
		{
			file = ((file_info *)p->datapointer);
			if(file->file_type == 'd')
			{
				strcpy(file_path,path);
				temp = strcat(file_path,"/");
				temp = strcat(file_path,file->file_name);
				recursion(temp,flag_a,flag_t,flag_s,flag_l,flag_r);
			}
		}
	}
	linkedlist_destory(head);

}

int add_file(N *head,char *file_path,char * file_name)
{
	struct stat buf;
	struct passwd *pw = NULL;
	struct group *gr = NULL;
	file_info *file;
	int j = 0;
	char *temp = NULL;
	char file_path_name[MAX_SIZE];
	struct tm *tm = NULL;
	int permission[9]=
	{
		S_IRUSR,S_IWUSR,S_IXUSR,

		S_IRGRP,S_IWGRP,S_IXGRP,

		S_IROTH,S_IWOTH,S_IXOTH
	};

	file = (file_info *)malloc(sizeof(file_info));
	if(file == NULL)
		return -1;
	file->permission[0] = 0;
	file->permission[1] = 0;
	file->permission[2] = 0;
	strcpy(file->file_name,file_name);
	strcpy(file->file_path,file_path);
	file->file_name_length = strlen(file_name);
	
	strcpy(file_path_name,file_path);
	temp = strcat(file_path_name,"/");
	temp = strcat(file_path_name,file_name);
	stat(temp,&buf);
	switch(buf.st_mode & S_IFMT)
	{
		case S_IFREG:
			file->file_type = '-';
			break;
		case S_IFDIR:
			file->file_type = 'd';
			break;
		case S_IFLNK:
			file->file_type = 'l';
			break;
		case S_IFBLK:
			file->file_type = 'b';
			break;
		case S_IFCHR:
			file->file_type = 'c';
			break;
		case S_IFIFO:
			file->file_type = 'p';
			break;
		case S_IFSOCK:
			file->file_type = 's';
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
					file->permission[(8-j)/3] += 4;
					break;
				case 1:
					file->permission[(8-j)/3] += 2;
					break;
				case 0:
					file->permission[(8-j)/3] += 1;
					break;
				default :
					break;
			}
		}
		else
		{
		}
	}

	pw = getpwuid(buf.st_uid);
	gr=getgrgid(buf.st_gid);
	strcpy(file->user,pw->pw_name);
	strcpy(file->group,gr->gr_name);
	file->file_size = buf.st_size;
	tm = localtime(&buf.st_ctime);
	file->ctime = buf.st_ctime;
	file->tm.tm_year = tm->tm_year;
	file->tm.tm_mon = tm->tm_mon;
	file->tm.tm_mday = tm->tm_mday;
	file->tm.tm_hour = tm->tm_hour;
	file->tm.tm_min = tm->tm_min;
	file->tm.tm_sec = tm->tm_sec;
	file->link = buf.st_nlink;
	add(head,(void *)file);
	return 0;

}

void show_l(N *head,int flag_l)
{
	N *p = NULL;
	int i = 0,temp_permission = 0;
	file_info *file = NULL;
	int count = 1;
	for(p = head->next;p != NULL;p = p->next)
	{
	file = (file_info *)(p->datapointer);
		if(flag_l == 1)
		{
		printf("%c",file->file_type);
		for(i = 0;i < 3;i++)
		{
			temp_permission = file->permission[i];
			if(temp_permission >= 4)
			{
				printf("r");
				temp_permission -= 4;
			}
			else
			{
				printf("-");
			}
			if(temp_permission >= 2)
			{
				printf("w");
				temp_permission -= 2;
			}
			else
			{
				printf("-");
			}
			if(temp_permission >= 1)
			{
				printf("x");
				temp_permission -= 1;
			}
			else
			{
				printf("-");
			}
		}
		printf("  %2d %-5s %-5s %5d %2d月 %2d %2d:%2d ",file->link,file->user,file->group,(int)file->file_size,
				file->tm.tm_mon+1,file->tm.tm_mday,file->tm.tm_hour,
				file->tm.tm_min);
		switch(file->file_type)
		{
			case 'd':
				printf("\033[34m%-10s\033[m\n",file->file_name);
				break;
			default :
				printf("%-10s\n",file->file_name);
				break;
		}
		}
		else
		{
			printf("%s  ",file->file_name);
			if(count%5 == 0)
			{
				printf("\n");
			}
			count ++ ;
		}
	}
	printf("\n");
}

int name_length_compare(void *a,void *b)
{
	file_info *temp_a = NULL,*temp_b = NULL;
	temp_a = (file_info *)a;
	temp_b = (file_info *)b;
	if(temp_a->file_name_length > temp_b->file_name_length)
	{
		return 1;
	}
	else if(temp_a->file_name_length == temp_b->file_name_length)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int file_name_compare(void *a,void *b)
{
	file_info *temp_a = NULL,*temp_b = NULL;
	temp_a = (file_info *)a;
	temp_b = (file_info *)b;
	return strcmp(temp_a->file_name,temp_b->file_name);
}

int file_size_compare(void *a,void *b)
{
	file_info *temp_a = NULL,*temp_b = NULL;
	temp_a = (file_info *)a;
	temp_b = (file_info *)b;
	if(temp_a->file_size > temp_b->file_size)
	{
		return 1;
	}
	else if(temp_a->file_size == temp_b->file_size)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int file_ctime_compare(void *a,void *b)
{
	file_info *temp_a = NULL,*temp_b = NULL;
	temp_a = (file_info *)a;
	temp_b = (file_info *)b;
	if(temp_a->ctime > temp_b->ctime)
	{
		return 1;
	}
	else if(temp_a->ctime == temp_b->ctime)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

