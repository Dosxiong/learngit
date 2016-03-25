#include <stdio.h>  
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <pcap/pcap.h>  
#include <arpa/inet.h>
#include "DoubleLink.h"
#include <time.h>
#include <pthread.h>

struct sniff_ip
{
	u_int   ip_v:4;                 ///版本
	u_int   ip_hl:4;				///头部长度
	u_char  ip_tos:8;               ///协议类型
	u_short ip_len:16;				///总长度
	u_short ip_id:16;               ///标示符id
	u_short ip_off:16;				///标志位加偏移量
	u_char  ip_ttl:8;				///生存时间
	u_char  ip_p:8;					///协议
	u_short ip_sum:16;				///校验和
	struct in_addr ip_src;
	struct in_addr ip_dst;
};

typedef struct second_link
{
	int id;
	int df;
	int mf;
	int offset;
	int len;
	u_char *packet;
}netpacket;

typedef struct information
{
	time_t start;
	int tol_len;				///总长度
	int cap_len;				///已捕获部分的长度
	int id;						///本分支的id
	int first_fregment;			///第一片是否到达
	int last_fregment;			///最后一片是否到达
	DLNode *Two_List;
}Info;

DLNode *head = NULL;
int temp_flag = 0;
u_char *new_packet = NULL;
static pthread_mutex_t testlock;
pthread_t test_thread;
int cap_count = 0, handle_count = 0;

void ip_recombination(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet);
Info * create_testlist();
int id_compare(const void *a, const void *b);
int offset_compare(const void *a, const void *b);
void deletepacket(DLNode *head);
void *test();

int main(int argc,char *argv[]){  
	char *dev, errbuf[PCAP_ERRBUF_SIZE];  
	int i = 0;
	struct bpf_program filter;
	char filter_app[] = "(ip[6:2] > 0) and (not ip[6] = 64)";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t *handle = NULL;
	pcap_dumper_t *pcap_dumper = NULL;
	DLNode *p = NULL, *p1 = NULL;

	head = CreateList();
	pthread_mutex_init(&testlock, NULL);
	//pthread_create(&test_thread, NULL, test, NULL);

	/*dev = pcap_lookupdev(errbuf);  
	if(dev == NULL)
	{  
		fprintf(stderr, "couldn't find default device: %s\n", errbuf);  
		return(2);  
	}  

	printf("Device: %s\n",dev);  
	pcap_lookupnet(dev, &net, &mask, errbuf);
	handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);*/
	handle = pcap_open_offline("frag_gn5-10.pcap", errbuf);

	pcap_compile(handle,&filter, filter_app, 0, net);
	pcap_setfilter(handle, &filter);
	pcap_dumper = pcap_dump_open(handle, "output.pcap");

	i = pcap_loop(handle, -1, ip_recombination, (u_char *)pcap_dumper);

	pcap_dump_flush(pcap_dumper);
	pcap_dump_close(pcap_dumper);
	for(p = head->next; p != head; )
	{
		p1 = p->next;
		DeleteList(p, deletepacket);
		p = p1;
	}
	free(head);
	head = NULL;
	p = NULL;
	p1 = NULL;

	printf("%-10d     %-10d*******\n",cap_count, handle_count);
	pcap_close(handle);

	return(0);  
}  

void ip_recombination(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	struct sniff_ip *ip_head = NULL, *new_head = NULL;
	netpacket *npacket = NULL, *temp_npacket = NULL;
	DLNode *p = NULL, *drop_p = NULL;
	u_char *temp_uchar = NULL, *new_uchar = NULL;
	int i = 0;
	struct pcap_pkthdr temp_pkthdr;
	int checksum = 0;
	Info *info = NULL;

	cap_count++;
	/*if(cap_count % 1000 == 0)
	{
		printf("cap_count:%-10d\n",cap_count);
	}*/

	npacket = (netpacket *)malloc(sizeof(netpacket));
	ip_head = (struct sniff_ip *) (packet + 14);
	npacket->id = ntohs(ip_head-> ip_id);
	npacket->df = ((ntohs(ip_head->ip_off) & 0x4000)  >>13);
	npacket->mf = ((ntohs(ip_head->ip_off) & 0x2000) >> 13);
	npacket->offset = (ntohs(ip_head-> ip_off) & 0x01fff)*8;
	npacket->len = (ntohs(ip_head->ip_len));

	new_uchar = (u_char *)malloc((sizeof(u_char)*npacket->len + 14));
	if(new_uchar != NULL)
	{
		memcpy(new_uchar, packet, (npacket->len + 14));
	}
	else
	{
		printf("new_uchar NULL\n");
	}
	npacket->packet = (u_char *)new_uchar;

	p = SearchList(head, (void *)npacket, id_compare);
	drop_p = p;
	if(p == head)
	{
		info = (Info *)malloc(sizeof(Info));
		info->id = npacket->id;
		info->first_fregment = 0;
		info->last_fregment = 0;
		info->tol_len = -1;
		info->cap_len = 0;
		info->Two_List = CreateList();

		//pthread_mutex_lock(&testlock); 
		InsertList(head, (void *)info);
		//pthread_mutex_unlock(&testlock); 

		drop_p = drop_p->next;

		//pthread_mutex_lock(&testlock); 
		InsertList(info->Two_List, (void *)npacket);
		//pthread_mutex_unlock(&testlock); 
	}
	else
	{
		info = (Info *)(p->data);
		p = SearchList(info->Two_List, (void *)npacket, offset_compare);

		//pthread_mutex_lock(&testlock); 
		InsertList(p->back, (void *)npacket);
		//pthread_mutex_unlock(&testlock); 
	}

	info->start = clock();
	if(npacket->mf == 0)
	{
		info->tol_len = npacket->offset + npacket->len - 20;
		info->last_fregment = 1;
	}
	if(npacket->offset == 0)
	{
		info->first_fregment = 1;
	}
	info->cap_len += npacket->len - 20;


	//printf("******* %-8d  %-8d\n",info->cap_len, info->tol_len);
	if((info->tol_len == info->cap_len) && (info->first_fregment == 1) && (info->last_fregment == 1))
	{
		handle_count ++;
		/*if(handle_count % 100 == 0)
		{
			printf("handle_count:%-10d\n",handle_count);
		}*/
		new_packet = (u_char *)malloc(sizeof(u_char)*(info->tol_len + 34));

		for(i = 0;i < 14;i ++)
		{
			new_packet[i] = packet[i];
		}

		new_head = (struct sniff_ip *)malloc((sizeof(struct sniff_ip)));
		memcpy(new_head, (packet + 14), 20);
		new_head->ip_len = htons(info->tol_len + 20);
		new_head->ip_off = 0;
		new_head->ip_sum = 0;
		temp_uchar = (u_char *)new_head;
		///检验和计算
		for(i = 0; i < 20; i = i+2)
		{
			checksum += (((temp_uchar[i]) << 8) | temp_uchar[i + 1]);
		}
		checksum = (checksum >> 16) + (checksum & 0xffff);     
		checksum += (checksum >> 16);     
		checksum = 0xffff - checksum; 
		/*checksum = ((checksum >> 16) & 0x00001)+(checksum & 0x0ffff);     
		  checksum=0xffff-checksum;*/   
		new_head->ip_sum = htons(checksum);

		memcpy((new_packet + 14), new_head, 20);
		temp_uchar = new_packet+34;

		for(p = (info->Two_List->next); p != info->Two_List; p = p->next)
		{
			temp_npacket = (netpacket *)(p->data);
			memcpy(temp_uchar, (temp_npacket->packet + 34), (temp_npacket->len - 20));
			temp_uchar = temp_uchar + temp_npacket->len -20;
		}

		temp_pkthdr.ts = pkthdr->ts;
		temp_pkthdr.caplen = info->cap_len;
		temp_pkthdr.len = info->cap_len;
		pcap_dump(arg, &temp_pkthdr, new_packet);
		free(new_packet);
		free(new_head);
		DeleteList(drop_p, deletepacket);

	}
	temp_uchar = NULL;
	temp_npacket = NULL;
	ip_head = NULL;

}

int id_compare(const void *a, const void *b)
{
	int result = 0;
	Info *temp_a = NULL;
	netpacket *temp_b = NULL;

	temp_a = (Info *)a;
	temp_b = (netpacket *)b;
	result = (temp_a->id - temp_b->id);
	temp_a = NULL;
	temp_b = NULL;
	return result;
}

int offset_compare(const void *a, const void *b)
{
	netpacket *temp_a = NULL, *temp_b = NULL;
	int result = 0;

	temp_a = (netpacket *)a;
	temp_b = (netpacket *)b;
	result = (temp_a->offset - temp_b->offset);
	temp_a = NULL;
	temp_b = NULL;
	if(result > 0)
	{
		return 0;
	}
	return 1;
}

Info * create_testlist()
{
	/*Info *p = NULL;
	  p = (Info *)malloc(sizeof(Info));
	  if(p != NULL)
	  {
	  p->tol_len = -1;
	  p->cap_len = 0;
	  p->first_fregment = 0;
	  p->last_fregment = 0;
	  p->list = NULL;
	  p->list = CreateList();
	  }*/
	return NULL;
}

void deletepacket(DLNode *head)
{
	DLNode *p = NULL, *p1 = NULL, *temp_head = NULL;
	netpacket *temp_npacket = NULL;
	Info *temp_info = NULL;

	temp_info = (Info *)(head->data);
	temp_head = temp_info->Two_List;
	for(p = temp_head->next;p != temp_head;)
	{
		temp_npacket = (netpacket *)(p->data);
		free(temp_npacket->packet);
		free(temp_npacket);
		temp_npacket = NULL;
		p1 = p;
		p = p->next;
		free(p1);
	}
	free(temp_info);
	free(temp_head);
}

/*void *test()
{
	DLNode *p = NULL;

	int i = 0;
	while(1)
	{
		pthread_mutex_lock(&testlock);
		p = head->next;
		//printf("test lock\n");
		while(p != head)
		{
			//printf("%d  time  %f\n", i, (float)(clock() - ((Info *)(p->data))->start) / CLOCKS_PER_SEC );
			i++;
			if( (float)(clock() - ((Info *)(p->data))->start) / CLOCKS_PER_SEC > 0.001 )
			{
				DeleteList(p, deletepacket);
			}
			else
			{
				p = p->next;
				continue;
			}
			p = p->next;
		}
		i = 0;
		pthread_mutex_unlock(&testlock);
		sleep(2);
	}

}*/
