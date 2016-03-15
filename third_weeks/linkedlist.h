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

#ifndef TEST1_H
#define TEST1_H

typedef struct node{
	struct node *next,*front;//双向指针
	void *datapointer;//数据指针
}N;

int add(N *head,void *data);

void int_show(N *head);

void string_show(N *head);

void float_show(N *head);

void callback_show(N *h,void (*show)(N *));

int int_compare(void *a,void *b);

int string_compare(void *a,void *b);

N *search(N *head,int (*compare)(void *,void *),void *value);

int del(N *head,int(*compare)(void *,void *),void *value);

int callback_modify(N *head,int (*modify)(N *,void *,void*),void *oldvalue,void *newvalue);

int int_modify(N *head,void *oldvalue,void *newvalue);

int string_modify(N *head,void *oldvalue,void *newvalue);

void callback_sort(N *head,void (*sort)(N *));

void int_sort(N *head);

void string_sort(N *head);

int statistics_value(N *head,int (*compare)(void *,void *),void *value);

int linkedlist_count(N *head);

void string_sort_xier(N *head,int (*compare)(void *,void *));

void linkedlist_destory(N * head);

N * create();

#endif //HELLO_H
