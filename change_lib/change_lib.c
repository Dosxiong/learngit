#include"DoubleLink.h"
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
void load(DLNode *head,char *);
void write_tofile(DLNode *head);
void help();

int article_line = 0;
int article_symbol = 0;
char * output_filename = NULL;

int main(int argc,char ** argv){
	DLNode head;
	int start = 0,end = 0;
	int i = 0;
	int flag_w = 0,flag_s = 0,flag_l = 0,flag_c = 0,flag_o = 0;
	int flag = 0; ///是否录入过文件
	int oc = 0; /// 选项字符
	char ec = 'l'; ///无效的选项字符
	char * b_opt_arg = NULL; ///选项参数字符串
	int potion_index = 0;
	static struct option long_options[] = {
		{"words",optional_argument,NULL,'w'},
		{"statistic",optional_argument,NULL,'s'},
		{"help",no_argument,NULL,'h'},
		{"outputfilename",optional_argument,NULL,'o'},
		{"byte",optional_argument,NULL,'c'},
		{"line",optional_argument,NULL,'l'}
	};

	output_filename = "output.txt";
	head.next = &head;
	head.back = &head;
	head.data= NULL;

	while((oc = getopt_long(argc,argv,":w::o::s::hl::c::t::",long_options,&potion_index)) != -1)
	{
		switch(oc)
		{
			case 'c':
				flag_c = 1;
				break;
			case 'h':
				help();
				flag = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'o':
				flag_o = 1;
				break;
			case 's':
				flag_s = 1;
				break;
			case 't':
				for(i = optind-1;i < argc;i++)
				{
					printf("%s\n",argv[i]);
				}
				printf("argc=%d,optind=%d\n",argc,optind);
				break;
			case 'w':
				flag_w = 1;
				break;
			case '?':
				ec = (char)optopt;
				printf("无效的字符\'%c\'\n",ec);
				flag = 1;
				break;
			case ':':
				flag = 1;
				printf("缺少选项参数\n");
				break;
		}
	}
	if(flag == 1)
		return 0;
	if(flag_o == 1)
	{
		if(flag_s == 0 && (flag_l == 1 || flag_w == 1 || flag_c == 1))
		{
			printf("-o必须和-s或者--statistic配合使用\n");
			return 0;
		}
		if(optind != argc)
		{
			output_filename = argv[optind];
			optind ++;
		}
	}
	if((flag_w+flag_l+flag_s+flag_c) == 0)
	{
		flag_w = flag_l = flag_s = flag_c = 1;
	}
		if(optind != argc)
		{
			for(i = optind;i < argc;i++)
			{
				printf("%s\n",argv[i]);
				load(&head,argv[i]);
				if(flag_c == 1)
				{
					printf("共有  %d  个字符\n",article_symbol);
					article_symbol = 0;
				}
				if(flag_l == 1)
				{
					article_symbol = 0;
					printf("共有  %d  行\n",article_line);
				}
				if(flag_w == 1)
				{
					printf("共有  %d  个单词\n",ShowList(&head,0,1));
				}
				if(flag_s == 1)
				{
					printf("************test1\n");
					SequenceList(&head,2,CallBackCmpStr);
					printf("************test2\n");
					write_tofile(&head);
					printf("结果已经输出到%s文件！\n",output_filename);
				}
				DropList(&head);
				head.next = &head;
				head.back = &head;
			}
		}
		else
		{
		}
	return 0;

}

void help()
{
	printf("\n文章统计指令说明\n");
	printf("\n");
	printf("参数\n");
	printf("\n  -h           --help                帮助信息\n");
	printf("\n  -w[filename] --words               统计单词个数\n");
	printf("\n  -s[filename] --statistic           词频统计\n");
	printf("\n  -o[filename] --statistic           指定输出文件（必须和-o结合和使用）\n");
	printf("\n  -l[filename] --line                词频文章的长度\n");
	printf("\n  -c[filename] --byte                词频字符数\n");


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

void write_tofile(DLNode *head)
{
	FILE * fp = NULL;
	DLNode * p = NULL;
	int count = 1;

	if((fp = fopen(output_filename,"w")) == NULL)
	{
		printf("can not find this file!\n");
		exit(0);
	}
	p = head->next->next;
	while(p != head)
	{
		if(CallBackCmpStr(p->back->data,p->data) == 0)
		{
			count++;
		}
		else
		{
			fprintf(fp,"%-20s %-10d\n",(char *)(p->back->data),count);
			count = 1;
		}
		if(p->next == NULL)
		{
			fprintf(fp,"%-20s %-10d\n",(char *)(p->data),count);
		}
		p = p->next;
	}
	fclose(fp);
}

void load(DLNode *head,char * file_name)
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
								InsertList(head,data);
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
									InsertList(head,(void *)data);
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
				InsertList(head,(void *)data);
				data = NULL;
			}
		}

	}
	fclose(fp);
}
