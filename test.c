#include"linkedlist.h"
#include<unistd.h>
#include<time.h>
#include<getopt.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_SIZE 1024
#define ASCII_A 65
#define ASCII_Z 90
#define ASCII_a 97
#define ASCII_z 122
#define ASCII_quotes 39

int is_letter(char ch);
void load(N *head,char *);
void write_tofile(N *head);
void help();

int article_line = 0;
int article_symbol = 0;

int main(int argc,char ** argv){
	N head;
	int start = 0,end = 0;
	int flag = 0; ///是否录入过文件
	int oc = 0; /// 选项字符
	char ec = 'l'; ///无效的选项字符
	char * b_opt_arg = NULL; ///选项参数字符串
	int potion_index = 0;
	static struct option long_options[] = {
		{"words",required_argument,NULL,'w'},
		{"statistic",required_argument,NULL,'s'},
		{"help",no_argument,NULL,'h'},
	};

	head.next = NULL;
	head.front = NULL;
	head.datapointer = NULL;

	while((oc = getopt_long(argc,argv,":w:s:hl:c:",long_options,&potion_index)) != -1)
	{
		switch(oc)
		{
			case 'c':
				if(flag == 0)
				{
					load(&head,optarg);
					flag = 1;
				}
				printf("%s  共有  %d  个字符\n",optarg,article_symbol);
				break;
			case 'w':
				if(flag == 0)
				{
					load(&head,optarg);
					flag = 1;
				}
				printf("%s  共有  %d  个单词\n",optarg,linkedlist_count(&head));
				break;
			case 's':
				if(flag == 0)
				{
					load(&head,optarg);
					flag = 1;
				}
				string_sort_xier(&head);
				write_tofile(&head);
				printf("结果已经输出到output.txt文件！\n");
				break;
			case 'h':
				help();
				break;
			case 'l':
				if(flag == 0)
				{
					load(&head,optarg);
					flag = 1;
				}
				printf("%s  共有  %d  行\n",optarg,article_line);
				break;
			case '?':
				ec = (char)optopt;
				printf("无效的字符（ %c ）\n",ec);
				break;
			case ':':
				printf("缺少选项参数\n");
				break;
		}
	}

	/*start = clock();
	load(&head);
	end = clock();
	printf("load time %f\n",(float)(end-start)/CLOCKS_PER_SEC);
	printf("words %d\n",linkedlist_count(&head));

	start = clock();
	string_sort_xier(&head);
	end = clock();
	printf("xier_sort time %f\n",(float)(end-start)/CLOCKS_PER_SEC);

	start = clock();
	write(&head);
	end = clock();
	printf("write to file time %f\n",(float)(end-start)/CLOCKS_PER_SEC);*/

	linkedlist_destory(&head);
	//callback_show(&head,string_show);

	/*data=(char *)malloc(sizeof(char)*10);
	  strcpy((char *)data,"hello");
	  printf("the amount is:%d\n",statistics_value(&head,string_compare,(void *)data));*/

	//string_statistics(&head,string_compare);

	return 0;
}

void help()
{
	printf("\n文章统计指令说明\n");
	printf("\n");
	printf("参数\n");
	printf("\n  -h                                帮助信息\n");
	printf("\n  -w[filename]                      统计单词个数 filename:要打开的文件名\n");
	printf("\n  -s[filename]                      词频统计 filename:要打开的文件名\n");
	printf("\n  -l[filename]                      词频文章的长度 filename:要打开的文件名\n");
	printf("\n  -c[filename]                      词频字符数 filename:要打开的文件名\n");
	printf("\n  --help                            帮助信息\n");
	printf("\n  --words[filename]                 统计单词个数 filename:要打开的文件名\n");
	printf("\n  --statistic[filename]             词频统计 filename:要打开的文件名\n");



}

int is_letter(char ch)
{
	int flag = -1;

	if((ch >= ASCII_a && ch <= ASCII_z))
	{
		flag = 1;
	}
	if((ch >= ASCII_A && ch <= ASCII_Z) )
	{
		flag = 2;
	}
	return flag;
}

void write_tofile(N *head)
{
	FILE * fp = NULL;
	N * p = NULL;
	int count = 1;

	if((fp = fopen("output.txt","w")) == NULL)
	{
		printf("can not find this file!\n");
		exit(0);
	}
	p = head->next->next;
	while(p != NULL)
	{
		if(string_compare(p->front->datapointer,p->datapointer) == 0)
		{
			count++;
		}
		else
		{
			fprintf(fp,"%-20s %-10d\n",(char *)(p->front->datapointer),count);
			count = 1;
		}
		if(p->next == NULL)
		{
			fprintf(fp,"%-20s %-10d\n",(char *)(p->datapointer),count);
		}
		p = p->next;
	}
	fclose(fp);
}

void load(N *head,char * file_name)
{
	char temp_buf[MAX_SIZE];
	FILE * fp = NULL;
	int i = 0,j = 0,k = 0;
	void * data = NULL;
	int count = 0;
	char temp_buf2[MAX_SIZE];


	temp_buf2[0] = '\0';
	if((fp = fopen(file_name,"r")) == NULL)
	{
		printf("can not find this file!\n");
		exit(0);
	}

	while((fgets(temp_buf,MAX_SIZE,fp) != NULL))
	{
		//printf("*******************************%d\n",count);
		//count++;
		article_line++;
		article_symbol ++;
		for(j = 0,i = 0;((temp_buf[i]) != EOF) && ((temp_buf[i]) != '\n'); i++)
		{
			if(temp_buf[i] == '\0')
			{
				article_line--;
				article_symbol--;
				for(k = 0;j < i;j++,k++,count++)
				{
					temp_buf2[k] = temp_buf[j];
				}
				temp_buf2[k] = '\0';
				i = j;
				break;
			}
			else
			{
				article_symbol ++;
				if(is_letter(temp_buf[i]) == -1)
				{
					if(i != 0 && temp_buf[i] == ASCII_quotes && is_letter(temp_buf[i-1]) > 0 && is_letter(temp_buf[i+1]) > 0)
					{
					}
					else
					{
						if(temp_buf2[0] != '\0')
						{
							data = (char *)malloc(sizeof(char)*(i-j+count+1));
							if(data != NULL)
							{
								//printf("**************(i-j+count):  %d    ",(i-j+count));
								for(k = 0;k < count;k++)
								{
									((char *)data)[k] = temp_buf2[k];
								}
								for(;j < i;j++,k++)
								{
									((char *)data)[k] = temp_buf[j];
								}
								((char *)data)[k] = '\0';
								add(head,(void *)data);
								//printf("换行 %s,length:%d\n",(char *)data,strlen((char *)data));
								data = NULL;
								temp_buf2[0] = '\0';
								count = 0;
							}
						}
						else
						{
							if(j != i)
							{
								data = (char *)malloc(sizeof(char)*(i-j+1));
								if(data != NULL)
								{
									//printf("(i-j+1):  %d   ",(i-j+1));
									for(k = 0;j < i;j++ ,k++)
									{
										((char *)data)[k] = temp_buf[j];
									}
									((char *)data)[k] = '\0';
									//printf("%d\n",strlen((char *)data));
									add(head,(void *)data);
									//printf("%s,length:%d\n",(char *)data,strlen((char *)data));
									data = NULL;
								}
							}
						}
						j++;
					}
				}
				else
				{
					if(is_letter(temp_buf[i]) == 2)
					{
						temp_buf[i]+=32;
					}
				}
			}
		}
		if(i != j)
		{
			temp_buf2[0] = '\0';
			data = (char *)malloc(sizeof(char)*(i-j+1));
			if(data != NULL)
			{
				for(k = 0;j < i;j++ ,k++)
				{
					((char *)data)[k] = temp_buf[j];
				}
				((char *)data)[k] = '\0';
				add(head,(void *)data);
				data = NULL;
			}
		}

	}
	fclose(fp);
}
