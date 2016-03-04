#include"linkedlist.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
	N head;
	int i=0;
	int returnvalue=0;
	void *data=NULL,*newdata=NULL;
	head.datapointer=NULL;
	head.next=NULL;
	head.front=NULL;

	for(i=0;i<10000;i++){
		data=(void *)malloc(sizeof( int));
		*(int *)data=rand()%1000;
		add(&head,(void *)data);

	}

	callbackshow(&head,int_show);
	/// 修改
	data=(void *)malloc(sizeof( int));
	newdata=(void *)malloc(sizeof( int));
	*(int *)data=1;
	*(int *)newdata=2;
	returnvalue=callbackmodify(&head,int_modify,(void *)data,(void *)newdata);
	if(returnvalue==1){
		free(data);
	}
	else{
		free(data);
		free(newdata);
	}
	printf("after modify function\n");

	callbackshow(&head,int_show);
	/// 删除
	data=(void *)malloc(sizeof( int));
	*(int *)data=1;
	del(&head,int_compare,(void *)data);
	printf("after del function\n");
	free(data);

	callbackshow(&head,int_show);
	/// 排序
	callbacksort(&head,int_sort);
	printf("after sort function\n");
	callbackshow(&head,int_show);
	return 0;
}

