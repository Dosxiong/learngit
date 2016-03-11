/**
* @file			DoubleLink.h  
* @brief		通用双向链表 \n
* 双向链表的动态创建，支持多种数据格式。
* @author		Sugarguo
* @email    	sugarguo@live.com
* @date			2016/03/01 20:00
* @version		v1.0.0 
* @copyright	EmbedWay 恒为科技武汉研发中心 By:Sugarguo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///@brief 定义双向链表的结构体
typedef struct _DoubleLink
{
	void *data; ///<双向链表中的数据
	struct _DoubleLink *back;  ///<双向链表的前区
	struct _DoubleLink *next;  ///<双向链表的后区
}DLNode;

/**
* CreateList	创建双向链表
* @return		返回值为List 为DLNode的空链表，首尾相连
*/
DLNode *CreateList()
{
	DLNode *List = NULL;
	List = (DLNode *)malloc(sizeof(DLNode));

	if(List == NULL)
	{
		printf ("Create MEM ERROR!\n");
	}
	else
	{
		List->back = List;
		List->next = List;

		return List;
	}
}

/**
* @brief CallBackCmpInt \n
* 回调函数，比较内容是否相同
* @param[in]	*one	比较的第一个内容
* @param[in]	*two	比较的第二个内容
* @return		返回值为两个数的减法结果，根据等于大小零判断
*/
int CallBackCmpInt(const void *one, const void *two)
{
	return(*(int *)one - *(int *)two);
}

/**
* @brief CallBackCmpChar \n
* 回调函数，比较内容是否相同
* @param[in]	*one	比较的第一个内容
* @param[in]	*two	比较的第二个内容
* @return		返回值为两个字母比较结果
*/
int CallBackCmpChar(const void *one, const void *two)
{
	return(*(char *)one - *(char *)two);
}
/**
* @brief CallBackCmpChar \n
* 回调函数，比较内容是否相同
* @param[in]	*one	比较的第一个内容
* @param[in]	*two	比较的第二个内容
* @return		返回值为两个单词比较结果
*/
int CallBackCmpStr(const void *one, const void *two)
{
	return(strcmp((char *)one, (char *)two));
}

/**
* @brief CallBackCmpStr_K \n
* 回调函数，比较内容是否相同
* @param[in]	*one	比较的第一个内容
* @param[in]	*two	比较的第二个内容
* @return		返回值为两个单词比较结果
*/
int CallBackCmpStr_K(const void *one, const void *two)
{
	return(strcmp(*(char **)one, *(char **)two));
}

/**
* @brief CallBackShow \n
* 回调函数，显示内容
* @param[in]	*one		需要显示的内容
* @param[in]	flag		提供接口，让用户定义具体比较函数
*/
int CallBackShow(void *one, int flag)
{
	if(flag == 0)
		printf("%d\n", *(int *)one);
	else if(flag == 1)
		printf("%c\n", *(char *)one);
	else if(flag == 2)
		printf("%s\n", (char *)one);
	else
		printf("请使用0:int 1:char 2:char *，目前仅支持这三种！\n");
}

/**
* @brief  Partition\n
* 快速排序 
* @param[in]	*List					双向链表默认链表接口
* @param[in]	*one					比较的第一个内容
* @param[in]	*two					比较的第二个内容
* @param[in]	*CallBackCmp			各种类型的回调函数接口
*/
DLNode *Partion(DLNode * List, DLNode * p_one, DLNode * p_two, int (*CallBackCmp)(const void *, const void *))
{
	void *temp = NULL;
	void *pivot = NULL;
	char *strtemp = NULL;
	if ( !List )
	{
		printf("ERROR ! HEAD NULL！/n");
		return ;
	}
	if ( !List->next )
	{
		return List->next;
	}

	pivot = p_one->data;

	while ( p_one != p_two )
	{
		//end -> start
		while ( p_one != p_two && CallBackCmp(p_two->data , pivot) >= 0 )
		{
			p_two = p_two->back;
		}

		temp = p_one->data;
		p_one->data = p_two->data;
		p_two->data = temp;
		//start -> end
		while ( p_one != p_two && CallBackCmp(p_one->data , pivot) <= 0 )
		{
			p_one = p_one->next;
		}

		temp = p_one->data;
		p_one->data = p_two->data;
		p_two->data = temp;
	}
	return p_one;
}

/**
* @brief  QuickSort\n
* 快速排序 
* @param[in]	*List					双向链表默认链表接口
* @param[in]	*one					比较的第一个内容
* @param[in]	*two					比较的第二个内容
* @param[in]	*CallBackCmp			各种类型的回调函数接口
*/
void QuickSort(DLNode * List, DLNode * p_one, DLNode * p_two, int (*CallBackCmp)(const void *, const void *))
{
	DLNode * p_temp = NULL;
	p_temp = Partion(List, p_one, p_two,CallBackCmp);
	if ( p_one != p_temp )
	{
		QuickSort(List, p_one, p_temp->back,CallBackCmp);
	}
	if ( p_two != p_temp )
	{
		QuickSort(List, p_temp->next, p_two,CallBackCmp);
	}
}

/**
* @brief SearchList \n
* 查询双向链表，通过给定的默认双向链表，判断数据以及数据地址是否存在
* @param[in]	*List					双向链表默认链表接口
* @param[in]	*data					需要查询的无类型数据指针
* @param[in]	*CallBackCmp			各种类型的回调函数接口
*/
DLNode *SearchList(DLNode *List, void *data, int (*CallBackCmp)(const void *, const void *))
{
	DLNode *p = NULL;

	p = List->next;
	while(p != List)
	{
		//CallBackShow(p->data, flag);
		if( CallBackCmp( p->data, data ) == 0 )
			return p;
		p = p->next;
	}
}

/**
* @brief SequenceList \n
* 排列双向链表函数通过flag的flag，选择合适的排序方法
* @param[in]	*List			双向链表默认链表接口
* @param[in]	flag					提供接口，让用户定义具体比较函数
* @param[in]	*CallBackCmp	各种类型的回调函数接口
*/
DLNode *SequenceList(DLNode *List, int flag, int (*CallBackCmp)(const void *,const void *))
{
	if(flag == 0)
	{
		DLNode *p_one = NULL, *p_two = NULL, *temp = NULL;
		void *temp_data = NULL;

		p_one = List->next;
		p_two = List->next;

		for(p_one = p_one; p_one->next != List; p_one = p_one->next)
		{
			temp=p_one;
			for(p_two = p_one->next; p_two != List; p_two = p_two->next)
			{
				if(CallBackCmp(p_two->data, temp->data) > 0 )
					temp = p_two;
			}
			if(temp != p_one)
			{
				temp_data=p_one->data;
				p_one->data=temp->data;
				temp->data=temp_data;
			}
		}
		//free(temp);
		//free(temp_data);
	}
	else if(flag == 1)
	{
		int Nodenum = 0;
		Nodenum = ShowList(List, 2, 1);

		char **StrList = NULL;
		StrList = (char**)malloc(Nodenum * sizeof(char*));
		if(StrList == NULL)
			return ;
		memset(StrList, 0, Nodenum * sizeof(char *));
		int i = 0;
		DLNode *p = NULL;

		p = List->next;
		//printf("back %s\n",(char *)p->data);
		while(p != List)
		{	
			StrList[i] = (char *)p->data;
			//printf("get   %s\n",StrList[i]);
			p = p->next;
			i++;
		}
		qsort(StrList, Nodenum, sizeof(char*), CallBackCmp);
		p = List->next;
		for(i = 0; i < Nodenum; i++)
		{
			//printf("qsort   %s\n",StrList[i]);
			p->data = StrList[i];
			p = p->next;
		}
		free(StrList);
	}
	else if(flag == 2)
	{
		QuickSort(List, List->next, List->back, CallBackCmp);
	}
	else if(flag == 3)
	{
		int nodenum = 0;
		nodenum = ShowList(List,2,1);
		int i = 0, j = 0, k = 0;
		DLNode *p_one = NULL;
		DLNode *p_two = NULL;

		for (k = nodenum / 2; k > 0; k /= 2)
		{
			p_one = p_one->next;
			p_two = NULL;
			for(i = 0; i < nodenum; i++ )
			{
				p_one = p_one->next;
				p_two = p_two->next;
				for (j = i + k; j < nodenum; j += k )
				{
					if(CallBackCmp(p_one->data,p_two->data) < 0 )
					{
						void *temp = NULL;
						temp = p_one->data;
						int l = j - k;
						while(l >= 0 && (CallBackCmp(p_two->data,temp) > 0))
						{
							p_one->data = p_two->data;
							p_two = p_two->next;
							l -= k;

						}
						
						p_one->data = temp;

					}
				}
			}
		}
	}
	else
	{
		printf("Error Choose!\n ONLY 0, 1\n");
	}
}

/**
* @brief InsertList \n
* 插入双向链表，根据双向链表的接口插入数据
* @param[in]	*List		双向链表默认链表接口
* @param[in]	*data		需要插入的无类型数据指针
*/
void InsertList(DLNode *List, void *data)
{
	DLNode *p = NULL;
	p = (DLNode *)malloc(sizeof(DLNode));

	if( p == NULL )
	{
		printf ("Create MEM ERROR!\n");
	}
	else
	{
		p->back = List;
		p->next = List->next;
		List->next->back = p;
		List->next = p;
		p->data = data;
	}
}

/**
* @brief UpdateList \n
* 更新双向链表，通过查询原始的节点位置，修改数据
* @param[in]	*List				双向链表默认链表接口
* @param[in]	*data_search		需要被更新的初始无类型数据指针
* @param[in]	*data_change		需要更新的无类型数据指针
* @param[in]	*CallBackCmp		各种类型的回调函数接口
*/
void UpdateList(DLNode *List, void *data_search, void *data_change, int (*CallBackCmp)(const void *, const void *))
{
	DLNode *p = NULL;

	p = SearchList(List, data_search, CallBackCmp);
	if(p != NULL)
	{
		p->data = data_change;
		printf("Update OK!\n");
		//free(p);
	}
	else
	{
		printf("ERROR Find REEOR!\n");
	}
}

/**
* @brief DeleteList \n
* 删除双向链表，查询节点位置，将前后地址相连接，然后free此节点位置
* @param[in]	*List			双向链表默认链表接口
* @param[in]	*data			需要被删除的无类型数据指针
* @param[in]	*CallBackCmp	各种类型的回调函数接口
*/
void DeleteList(DLNode *List, void *data, int (*CallBackCmp)(const void *, const void *))
{
	DLNode *p = NULL;
	p = (DLNode *)malloc(sizeof(DLNode));

	if( p == NULL )
	{
		printf ("Create MEM ERROR!\n");
	}
	else
	{
		p = SearchList(List, data, CallBackCmp);
		p->back->next = p->next;
		p->next->back = p->back;
		free(p->data);
		p->data = NULL;
		DLNode *temp = NULL;
		temp = p;
		p = p->next;
		free(temp);
		if( SearchList(List, data, CallBackCmp ) == 0)
		{
			printf("Delete Error!\n");
		}
		else
			printf("Delete OK!\n");	
	}
}

/**
* @brief ShowList \n
* 遍历双向链表
* @param[in]	*List	双向链表默认链表接口
* @param[in]	flag	提供接口，让用户定义具体比较函数
* @param[in]	judge	提供接口，让用户定义是否显示遍历结果
* @return		返回值为遍历结果，有多少个
*/
int ShowList(DLNode *List, int flag, int judge)
{
	printf("Show List!\n");
	DLNode *p;
	int Nodenum = 0;

	p = List->next;
	while(p != List)
	{
		if(judge != 1)
			CallBackShow(p->data, flag);
		p = p->next;
		Nodenum++;
	}
	printf("LinkList Has %d \n",Nodenum);
	return Nodenum;
}

/**
* @brief DropList \n
* 删除双向链表，全部删除，释放内存空间
* @param[in]	*List	双向链表默认链表接口
*/
void DropList(DLNode *List)
{
	DLNode *p = NULL;

	p = List->next;
	while(p != List)
	{
		DLNode *temp = NULL;
		temp = p;
		free(temp->data);
		temp->data = NULL;
		p = p->next;
		free(temp);
	}
	//free(List);
}

