#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <pcap/pcap.h>  
int main(int argc,char *argv[]){  
	char *dev, errbuf[PCAP_ERRBUF_SIZE];  
	int i = 0;
	struct bpf_program filter;
	char filter_app[] = "src host  192.168.8.144 && arp ";
	bpf_u_int32 mask;
	bpf_u_int32 net;
	pcap_t *handle = NULL;
	pcap_dumper_t *pcap_dumper = NULL;
	dev=pcap_lookupdev(errbuf);  
	if(dev==NULL){  
		fprintf(stderr,"couldn't find default device: %s\n",errbuf);  
		return(2);  
	}  
	printf("Device: %s\n",dev);  
	pcap_lookupnet(dev,&net,&mask,errbuf);
	handle = pcap_open_live(dev,BUFSIZ,1,0,errbuf);
	pcap_compile(handle,&filter,filter_app,0,net);
	pcap_setfilter(handle,&filter);
	pcap_dumper = pcap_dump_open(handle,"libcaptest1.pcap");
	printf("%d*******\n",i);
	i = pcap_loop(handle,10,pcap_dump,(u_char *)pcap_dumper);
	pcap_dump_flush(pcap_dumper);
	pcap_dump_close(pcap_dumper);
	
	printf("%d*******\n",i);
	pcap_close(handle);
	return(0);  
}  
