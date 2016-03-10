//////////////////////////////////////////////////////////////////////////  

///     COPYRIGHT NOTICE  

///     Copyright (c) 2016, 恒为科技股份有限公司  

///     All rights reserved.  

///  

/// @file             linkedlist.c 

/// @brief            本文件是一个实现了增删改查排序的链表库  

///  

///

///  

/// @version 1.1        

/// @author           熊锐  

/// @date             2016-3-4  

///  

///  

///     修订说明：最初版本  

//////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node
{
	struct node *next,*front;     ///双向指针,next指向后面结点,front指向前面结点
	void *datapointer;           ///数据指针
}N;

///      下面是一个显示回调函数   

///     

///      显示回调函数用于不同类型的数据的显示     

///      @param h 要显示的链表的头指针  

///      @param void (*show)(N *) 需要回调的函数指针    

///      @return     void   

///      @see       

///      @note    

void callback_show(N *h,void (*show)(N *))
{
	show(h);
}

///      下面是一个int类型的显示函数   

///     

///      遍历链表显示int类型的数据     

///      @param h 要显示的链表的头指针  

///      @return     void   

///      @see       

///      @note

void int_show(N *head)
{
	N *p;

	for(p = head->next;p != NULL;p = p->next)
	{
		printf("the data is :%d\n",*(int *)(p->datapointer));
	}
}

///      下面是一个float类型的显示函数   

///     

///      遍历链表显示float类型的数据     

///      @param h 要显示的链表的头指针  

///      @return     void   

///      @see       

///      @note

void float_show(N *head)
{
	N *p=NULL;

	for(p = head->next;p != NULL;p = p->next)
	{
		printf("the data is :%f\n",*(float *)(p->datapointer));
	}
}

///      下面是一个string类型的显示函数   

///     

///      遍历链表显示string类型的数据     

///      @param h 要显示的链表的头指针  

///      @return     void   

///      @see       

///      @note

void string_show(N *head)
{
	N *p;

	for(p = head->next;p != NULL;p = p->next)
	{
		printf("the data is :%s\n",(char *)(p->datapointer));
	}
}

///      下面是一个增加结点的函数   

///     

///      用于链表增加数据     

///      @param h 要显示的链表的头指针 

///      @param data 要增加的数据指针  

///      @return     int 如果成功返回1,不成功返回0   

///      @see       

///      @note

int add(N *head,void *data)
{
	N *p = (N *)malloc(sizeof(N));

	if(p != NULL)
	{
		p->next = NULL;
		p->front = NULL;
		p->datapointer = data;

		if(head->next == NULL)
		{
			head->next = p;
			p->front = head;
		}
		else
		{
			p->next = head->next;
			p->front = head;
			head->next->front = p;
			head->next = p;
		}
		return 1;
	}
	return 0;
}

///      下面是一个int类型数据的比较函数 

///     

///      用于int类型数据的比较    

///      @param a 要比较的数据指针

///      @param b 要比较的数据指针  

///      @return     int 如果a=b返回0,如果a>b返回1,如果a<b返回-1   

///      @see       

///      @note

int int_compare(void *a,void *b)
{
	if(*(int *)a == *(int *)b)
	{
		return 0;
	}
	else if(*(int *)a > *(int *)b)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

///      下面是一个string类型数据的比较函数 

///     

///      用于string类型数据的比较    

///      @param a 要比较的数据指针

///      @param b 要比较的数据指针  

///      @return     int 如果a=b返回0,如果a>b返回1,如果a<b返回-1   

///      @see       

///      @note

int string_compare(void *a,void *b)
{
	return strcmp((char *)a,(char *)b);
}

///      下面是一个查找函数 

///     

///      给点链表头和要查找的数据查找   

///      @param head 链表的头指针

///      @param (*compare)(void *,void *) 要回调的比较函数 

///      @param value 要查找的数据指针  

///      @return     N * 如果找到改结点则返回,没找到返回NULL   

///      @see       

///      @note

N *search(N *head,int (*compare)(void *,void *),void *value)
{
	N *p = head->next;

	while(p != NULL)
	{
		if(compare(p->datapointer,value) == 0)
		{
			break;
		}
		p = p->next;
	}
	return p;
}


///     下面是一个查找有多少个相同结点的函数

///      @param head 链表的头指针

///      @param (*compare)(void *,void *) 要回调的比较函数 

///      @param value 要查找的数据指针  

///      @return      返回找到的结点的个数 

///      @see       

///      @note

int statistics_value(N *head,int (*compare)(void *,void *),void *value)
{
	N *p = head->next;
	int count = 0;

	while(p != NULL)
	{
		if(compare(p->datapointer,value) == 0)
		{
			count++;
		}
		p = p->next;
	}
	return count;
}


///      下面是一个删除函数 

///     

///      根据数据指针删除数据   

///      @param head 链表的头指针

///      @param (*compare)(void *,void *) 要回调的比较函数 

///      @param value 要删除的数据指针  

///      @return     int 如果删除成功返回1 否则返回0   

///      @see       

///      @note

int del(N *head,int(*compare)(void *,void *),void *value)
{
	N *p = head->next;

	//printf("1\n");
	while(p != NULL){
		if(compare(p->datapointer,value) == 0)
		{
			p->front->next = p->next;
			if(p->next != NULL)
			{
				p->next->front = p->front;
			}
			free(p->datapointer);
			free(p);
			//	printf("2\n");
			p = NULL;
			return 1;
		}
		//	printf("3\n");
		p = p->next;
	}
	return 0;
} 

///      下面是一个修改回调函数

///     

///      根据给定的值查找,然后给定的值修改   

///      @param head 链表的头指针

///      @param (*compare)(void *,void *) 要回调的比较函数 

///      @param oldvalue 要修改的数据指针

///      @param newvalue 新的数据指针  

///      @return     N * 如果修改成功则返回1,否则0

///      @see       

///      @note

int callback_modify(N *head,int (*modify)(N *,void *,void*),void *oldvalue,void *newvalue)
{
	return modify(head,oldvalue,newvalue);
}

///      下面是一个int类型的修改函数

///     

///      根据给定的值查找,然后给定的值修改   

///      @param head 链表的头指针

///      @param oldvalue 要修改的数据指针

///      @param newvalue 新的数据指针  

///      @return     N * 如果修改成功则返回1,否则0

///      @see       

///      @note

int int_modify(N *head,void *oldvalue,void *newvalue)
{
	N *p = search(head,int_compare,oldvalue);

	if(p != NULL)
	{
		free(p->datapointer);
		p->datapointer = newvalue;
		return 1;
	}
	else
	{
		return 0;
	}
}

///      下面是一个string类型的修改函数

///     

///      根据给定的值查找,然后给定的值修改   

///      @param head 链表的头指针

///      @param oldvalue 要修改的数据指针

///      @param newvalue 新的数据指针  

///      @return     N * 如果修改成功则返回1,否则0

///      @see       

///      @note

int string_modify(N *head,void *oldvalue,void *newvalue)
{
	N *p = search(head,string_compare,oldvalue);

	if(p != NULL)
	{
		free(p->datapointer);
		p->datapointer = newvalue;
		return 1;
	}
	else
	{
		return 0;
	}
}

///      下面是一个排序回调函数

///     

///      按照降序排列  

///      @param head 链表的头指针

///      @param (*sort)(N *) 要回调的排序函数

///      @return     void

///      @see       

///      @note

void callback_sort(N *head,void (*sort)(N *))
{
	sort(head);
}

///      下面是一个int类型的排序函数

///     

///      按照降序排列  

///      @param head 链表的头指针

///      @return     void

///      @see       

///      @note

void int_sort(N *head)
{
	N *p = NULL,*p1 = NULL,*p2 = NULL;
	void *temp = NULL;

	for(p = head->next;p->next != NULL;p = p->next)
	{
		p2 = p;
		for(p1 = p->next;p1 != NULL;p1 = p1->next)
		{
			if(*(int *)(p1->datapointer) > *(int *)(p2->datapointer))
			{
				p2 = p1;
			}
		}
		if(p2 != p)
		{
			temp = p->datapointer;
			p->datapointer = p2->datapointer;
			p2->datapointer = temp;
		}
	}
}

///      下面是一个统计个数函数

///     

///      @param head 链表的头指针

///      @return     返回链表的结点个数

///      @see       

///      @note

int linkedlist_count(N *head)
{
	N * p = NULL;
	int count = 0;

	for(p = head->next;p != NULL;p = p->next,count++)
	{
	}
	return count;
}


///      下面是一个string类型的希尔排序函数

///     

///      按照降序排列  

///      @param head 链表的头指针

///      @return     void

///      @see       

///      @note

void string_sort_xier(N *head)
{
	N ** linkedlist_array = NULL;
	N * p = NULL;
	int i = 0,j = 0;;
	int k = 0;
	int count = 0,increment = 0;
	void * temp_pointer = NULL;

	count = linkedlist_count(head);
	linkedlist_array = (N **)malloc(sizeof(N * )*(count));	
	if(linkedlist_array != NULL)
	{
		for(p = head->next,i = 0;p !=NULL;p = p->next,i++)
		{
			linkedlist_array[i] = (N *)p;
		}
		increment = count;
		while(increment > 1)
		{
			k++;
			increment = increment/3+1;
			for(i = increment;i < count;i++)
			{
				if(string_compare(linkedlist_array[i]->datapointer,linkedlist_array[i-increment]->datapointer) < 0)
				{
					temp_pointer = (void *)(linkedlist_array[i]->datapointer); /// 
					for(j = i-increment;j >= 0 &&string_compare(temp_pointer,linkedlist_array[j]->datapointer) < 0;j = j-increment)
					{
						linkedlist_array[j+increment]->datapointer = linkedlist_array[j]->datapointer;
					}
					linkedlist_array[j+increment]->datapointer = temp_pointer;
				}
			}
		}
	}
	if(linkedlist_array != NULL)
	{
		free(linkedlist_array);
		linkedlist_array = NULL;
	}
}


///      下面是一个string类型的排序函数

///     

///      按照降序排列  

///      @param head 链表的头指针

///      @return     void

///      @see       

///      @note

void string_sort(N *head)
{
	N *p = NULL,*p1 = NULL,*p2 = NULL;
	void *temp = NULL;

	for(p = head->next;p->next != NULL;p = p->next)
	{
		p2 = p;
		for(p1 = p->next;p1 != NULL;p1 = p1->next)
		{
			if(strcmp((char *)p1->datapointer,(char *)p2->datapointer) > 0)
			{
				p2 = p1;
			}
		}
		if(p2 != p)
		{
			temp = p->datapointer;
			p->datapointer = p2->datapointer;
			p2->datapointer = temp;
		}
	}
}


///      下面是一个销毁函数


///      @param head 链表的头指针

///      @return     void

///      @see       

///      @note

void linkedlist_destory(N * head)
{
	N * p = NULL,* p1 = NULL;

	if(head->next != NULL)
	{
	p = head->next;
	p1 = p->next;
	while(p1 != NULL)
	{
		free(p->datapointer);
		free(p);
		p = NULL;
		p = p1;
		p1 = p1->next;
	}
	free(p->datapointer);
	free(p);
	p = NULL;
	head->next = NULL;
	}
}
