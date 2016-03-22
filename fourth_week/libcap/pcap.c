#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <pcap/pcap.h>  
#include<arpa/inet.h>
#include"DoubleLink.h"

void ip_recombination(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet);

struct sniff_ip
{
	u_int   ip_v:4;                  ///版本
	u_int   ip_hl:4;				 //头部长度
	u_char  ip_tos:8;              
	u_short ip_len:16;
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
	u_char *packet;
}netpacket;

DoubleLink *head;

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
	struct sniff_ip *head = NULL;
	netpacket *npacket = NULL;
	DoubleLink *p = NULL;

	npacket = (netpacket *)malloc(sizeof(netpacket));
	head = (struct sniff_ip *) (packet + 14);
	npacket->id = ntohs(head-> ip_id);
	npacket->df = ((ntohs(head->ip_off) & 0x4000)  >>13);
	npacket->mf = ((ntohs(head->ip_off) & 0x2000) >> 13);
	mpacket->offset = (ntohs(head-> ip_off) & 0x01fff)*8;
	mpacket->packet = packet;

	p = SearchList(npacket, offset_compare);
	if(p == NULL)
	{
		p = CreateList();
		InsertList(head, p);
		InsertList(p, npacket);
	}
	else
	{
		InsertList(p, npacket);
		if(all_packet(p) == 0)
		{
		}
	}

	printf("%-8d %-2d %-2d %-8d %0x\n",id, df, mf, offset, ntohs(head->ip_off));

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

int all_packet(DoubleLink *head)
{
	return 0;
}
