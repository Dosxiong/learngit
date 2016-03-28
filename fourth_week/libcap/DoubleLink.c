/**
* @file			DoubleLink.h  
* @brief		通用双向链表 \n
* 双向链表的动态创建，增删改查，支持多种数据格式，提供冒泡 快排方法。
* @author		Sugarguo
* @email    	sugarguo@live.com
* @date			2016/03/08 20:00
* @version		v2.0.0 
* @copyright	EmbedWay 恒为科技武汉研发中心 By:Sugarguo
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
* @brief 定义双向链表的结构体
* @param[in]	*data	双向链表中的数据
* @param[in]	*back	返回指针
* @param[in]	*next	前进指针
*/
typedef struct _DoubleLink
{
	void *data; ///<双向链表中的数据
	struct _DoubleLink *back;  ///<双向链表的前区
	struct _DoubleLink *next;  ///<双向链表的后区
}DLNode;


/**
 * @brief CreateList \n
 * 创建双向链表
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * null      | null      |null
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * *List     | DLNode    |双向链表默认链表接口
 * @warning   null
 * @attention null
 * @note      创建双向链表,返回指针
 * @todo      null
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
* @brief CallBackCmpStr \n
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
* @brief CallBackShowInt \n
* 回调函数，显示内容
* @param[in]	*data		需要显示的内容
*/
void CallBackShowInt(void *data)
{
	printf("%d\n", *(int *)data);
}
/**
* @brief CallBackShowChar \n
* 回调函数，显示内容
* @param[in]	*data		需要显示的内容
*/
void CallBackShowChar(void *data)
{
	printf("%c\n", *(char *)data);
}
/**
* @brief CallBackShowStr \n
* 回调函数，显示内容
* @param[in]	*data		需要显示的内容
*/
void CallBackShowStr(void *data)
{
	printf("%s\n", (char *)data);
}
/**
* @brief CallBackDeleteNode \n
* 回调函数，删除Node
* @param[in]	*p		需要删除的内容
*/
void CallBackDeleteNode(DLNode *p)
{
	p->back->next = p->next;
	p->next->back = p->back;
	free(p->data);
	p->data = NULL;
	DLNode *temp = NULL;
	temp = p;
	p = p->next;
	free(temp);
}
/**
* @brief CallBackDropList \n
* 回调函数，删除List
* @param[in]	*List		需要删除的内容
*/
void CallBackDropList(DLNode * temp)
{
	free(temp->data);
	temp->data = NULL;
}


/**
* @brief  Partition\n
* 快速排序 
* @param[in]	*List					双向链表默认链表接口
* @param[in]	*one					比较的第一个内容
* @param[in]	*two					比较的第二个内容
* @param[in]	*CallBackCmp			各种类型的回调函数接口
*/
static DLNode *Partion(DLNode * List, DLNode * p_one, DLNode * p_two, int (*CallBackCmp)(const void *, const void *))
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
static void QuickSort(DLNode * List, DLNode * p_one, DLNode * p_two, int (*CallBackCmp)(const void *, const void *))
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
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List       | DLNode    |双向链表默认链表接口
 * *data       | void      |需要查询的数据指针
 * *CallBackCmp| int       |判断回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   null
 * @attention null
 * @note      查询双向链表，通过给定的默认双向链表，判断数据以及数据地址是否存在
 * @todo      null
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
	return p;
}


/**
 * @brief SequenceList \n
 * 排列双向链表函数通过flag的flag，选择合适的排序方法
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List       | DLNode    |双向链表默认链表接口
 * flag        | int       |采用排序方法的flag
 * *CallBackCmp| int       |判断回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   null
 * @attention 0： 为冒泡排序，耗费较多资源；1： 系统快排，最快 2： 自写快排
 * @note      排列双向链表函数通过flag的flag，选择合适的排序方法
 * @todo      null
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
	else
	{
		printf("Error Choose!\n ONLY 0, 1\n");
	}
}


/**
 * @brief InsertList \n
 * 插入双向链表，根据双向链表的接口插入数据
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List     | DLNode    |双向链表默认链表接口
 * *data     | void      |需要插入的数据指针
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   null
 * @attention null
 * @note      插入双向链表，根据双向链表的接口插入数据
 * @todo      null
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
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List       | DLNode    |双向链表默认链表接口
 * *data_search| void      |需要查询的数据指针
 * *data_change| void      |需要修改的数据指针
 * *CallBackCmp| int       |判断回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   更新节点操作，一经操作无法返回，慎用！
 * @attention null
 * @note      更新双向链表，通过查询原始的节点位置，修改数据
 * @todo      null
 */
void UpdateList(DLNode *List, void *data_search, void *data_change, int (*CallBackCmp)(const void *, const void *))
{
	DLNode *p = NULL;

	p = SearchList(List, data_search, CallBackCmp);
	if(p != NULL)
	{
		p->data = data_change;
		//printf("Update OK!\n");
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
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List          | DLNode    |双向链表默认链表接口
 * *data          | void      |显示格式flag
 * *CallBackCmp   | int       |判断回调函数
 * *CallBackDelete| int       |删除回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   删除节点操作，一经操作无法返回，慎用！
 * @attention null
 * @note      删除双向链表，查询节点位置，将前后地址相连接，然后free此节点位置
 * @todo      null
 */
int DeleteList(DLNode *List,  void (*CallBackDelete)(DLNode *))
{
	DLNode *p = List;

	if(p != NULL)
	{
		p->back->next = p->next;
		p->next->back = p->back;
		CallBackDelete(p);
		//free(p->data);
		free(p);
	}
	return 0;
}


/**
 * @brief ShowList \n
 * 遍历双向链表，根据flag选择显示格式，根据judge选择是否输出遍历结果
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List        | DLNode    |双向链表默认链表接口
 * judge        | int       |是否显示flag
 * *CallBackShow| void      |显示回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * nodenum   | int       | 返回链表长度
 * @warning   null
 * @attention null
 * @note      遍历双向链表，根据flag选择显示格式，根据judge选择是否输出遍历结果
 * @todo      null
 */
int ShowList(DLNode *List, int judge, void (*CallBackShow)(const void *))
{
	//printf("Show List!\n");
	DLNode *p;
	int nodenum = 0;

	p = List->next;
	while(p != List)
	{
		if(judge != 1)
			CallBackShow(p->data);
		p = p->next;
		nodenum++;
	}
	//printf("LinkList Has %d \n",Nodenum);
	return nodenum;
}


/**
 * @brief DropList \n
 * 删除双向链表，全部删除，释放内存空间
 * @date   2016-03-11
 * @author Sugarguo
 * @param  : 参数说明如下表：
 * name      | type      |description of param 
 * ----------|-----------|--------------------
 * *List        | DLNode    |双向链表默认链表接口
 * *CallBackDrop| void      |删除的回调函数
 * @return    返回值说明如下：
 * name      | type      | description of value
 * ----------|-----------|----------------------
 * null      | null      | null
 * @warning   此操作会清空双向链表的数据，系统暂存空间
 * @attention null
 * @note      此操作会清空双向链表的数据，系统暂存空间,谨慎操作
 * @todo      null
 */
void DropList(DLNode *List, void (*CallBackDrop)(DLNode *))
{
	DLNode *p = NULL;

	p = List->next;
	while(p != List)
	{
		DLNode *temp = NULL;
		temp = p;
		CallBackDrop(temp);
		p->back->next = p->next;
		p->next->back = p->back;
		p = p->next;
		free(temp);
	}
	free(List);
	//List = NULL;
}


