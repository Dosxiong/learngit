#include <stdio.h>  
#include<string.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <pcap/pcap.h>  
#include<arpa/inet.h>
#include"DoubleLink.h"

struct sniff_ip
{
	u_int   ip_v:4;                 ///版本
	u_int   ip_hl:4;				///头部长度
	u_char  ip_tos:8;               ///协议类型
	u_short ip_len:16;				///总长度
	u_short ip_id:16;               ///标示符id
	u_short ip_off:16;
	u_char  ip_ttl:8;
	u_char  ip_p:8;
	u_short ip_sum:16;
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
	int tol_len;				///总长度
	int cap_len;				///已捕获部分的长度
	int id;						///本分支的id
	int first_fregment;			///第一片是否到达
	int last_fregment;			///最后一片是否到达
	DLNode *list;
}Info;

DLNode *head = NULL, *last = NULL;
Info *info = NULL;
int temp_flag = 0;
u_char *new_packet = NULL;


void ip_recombination(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet);
Info * create_testlist();

int main(int argc,char *argv[]){  
	char *dev, errbuf[PCAP_ERRBUF_SIZE];  
	int i = 0;
	struct bpf_program filter;
	char filter_app[] = "(ip[6:2] > 0) and (not ip[6] = 64)";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t *handle = NULL;
	pcap_dumper_t *pcap_dumper = NULL;

	head = CreateList();
	info = create_testlist();
	last = info->list;

	dev = pcap_lookupdev(errbuf);  
	if(dev == NULL)
	{  
		fprintf(stderr, "couldn't find default device: %s\n", errbuf);  
		return(2);  
	}  

	printf("Device: %s\n",dev);  
	pcap_lookupnet(dev, &net, &mask, errbuf);
	handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);

	pcap_compile(handle,&filter, filter_app, 0, net);
	pcap_setfilter(handle, &filter);
	pcap_dumper = pcap_dump_open(handle, "libcaptest1.pcap");

	i = pcap_loop(handle, 50, ip_recombination, (u_char *)pcap_dumper);

	pcap_dump_flush(pcap_dumper);
	pcap_dump_close(pcap_dumper);

	printf("%d*******\n",i);
	pcap_close(handle);

	return(0);  
}  

void ip_recombination(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	struct sniff_ip *head = NULL, *new_head = NULL;
	netpacket *npacket = NULL, *temp_npacket = NULL;
	DLNode *p = NULL;
	u_char *temp_uchar = NULL;
	int i = 0;
	struct pcap_pkthdr temp_pkthdr;
	int checksum = 0;

	npacket = (netpacket *)malloc(sizeof(netpacket));
	head = (struct sniff_ip *) (packet + 14);
	npacket->id = ntohs(head-> ip_id);
	npacket->df = ((ntohs(head->ip_off) & 0x4000)  >>13);
	npacket->mf = ((ntohs(head->ip_off) & 0x2000) >> 13);
	npacket->offset = (ntohs(head-> ip_off) & 0x01fff)*8;
	npacket->len = (ntohs(head->ip_len));
	npacket->packet = (u_char *)packet;

	if(temp_flag == 0)
	{
		temp_flag = 1;
		info->id = npacket->id;
	}
	if(info->id == npacket->id)
	{
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
		InsertList(last, npacket);
		last = last->next;


		printf("%-8d  %-8d\n",info->cap_len, info->tol_len);
		if(info->tol_len == info->cap_len)
		{
			new_packet = (u_char *)malloc(sizeof(u_char)*(info->tol_len + 34));
			p = info->list->next;
			printf("all packet\n");

			for(i = 0;i < 14;i ++)
			{
				new_packet[i] = packet[i];
			}

			new_head = (struct sniff_ip *)malloc(sizeof(struct sniff_ip));
			memcpy(new_head, (packet + 14), 20);
			new_head->ip_len = htons(info->tol_len + 20);
			new_head->ip_off = 0;
			new_head->ip_sum = 0;
			temp_uchar = (u_char *)new_head;
			printf("%04x\n", new_head->ip_sum);
			///检验和计算
			for(i = 0; i < 20; i = i+2)
			{
				checksum += (((temp_uchar[i]) << 8) | temp_uchar[i + 1]);
				printf("%d  %04x\n", i, (((temp_uchar[i]) << 8) | temp_uchar[i + 1]));
			}
			checksum=(checksum>>16)+(checksum & 0xffff);     
			checksum+=(checksum>>16);     
			checksum=0xffff-checksum; 
			/*checksum = ((checksum >> 16) & 0x00001)+(checksum & 0x0ffff);     
			checksum=0xffff-checksum*/;   
			new_head->ip_sum = htons(checksum);

			memcpy((new_packet + 14), new_head, 20);
			temp_uchar = new_packet+34;

			for(;p != info->list;p = p->next)
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
			printf("all packet\n");
		}
	}


	printf("%-8d %-2d %-2d %-8d %-8d\n",npacket->id, npacket->df, npacket->mf, npacket->offset, npacket->len);

	head = NULL;

	pcap_dump(arg, pkthdr, packet);
}

int offset_compare(void *a, void *b)
{
	netpacket *temp_a = NULL, *temp_b = NULL;
	temp_a = (netpacket *)a;
	temp_b = (netpacket *)b;
	if(temp_a->offset > temp_b->offset)
	{
		return 1;
	}
	if(temp_a->offset = temp_b->offset)
	{
		return 0;
	}
	if(temp_a->offset < temp_b->offset)
	{
		return -1;
	}
}

Info * create_testlist()
{
	Info *p = NULL;
	p = (Info *)malloc(sizeof(Info));
	if(p != NULL)
	{
		p->tol_len = -1;
		p->cap_len = 0;
		p->first_fregment = 0;
		p->last_fregment = 0;
		p->list = NULL;
		p->list = CreateList();
	}
	return p;
}

