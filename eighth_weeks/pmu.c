#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>
#include "zlog.h"
#include "message.h"
#include "cmdline.h"

typedef enum
{
	SUCCESS = 0,
	FAIL = -1,
}return_value_t;

int rul_add_req (rule_req_t *packet, int seq, options_t op)
{
	int i = 0;

	packet->msg_type = op.msg_type; 
	packet->msg_version = op.msg_version;
	packet->grp_id = op.grp_id;
	packet->msg_len = htons (sizeof(rule_req_t));
	packet->seqs = htonl (seq);
	packet->outer_src_ip = op.outer_src_ip.s_addr;
	packet->outer_dst_ip = op.outer_dst_ip.s_addr;
	packet->inner_src_ip = op.inner_src_ip.s_addr;
	packet->inner_dst_ip = op.inner_dst_ip.s_addr;
	packet->outer_src_port = op.outer_src_port;
	packet->outer_dst_port = op.outer_dst_port;
	packet->inner_src_port = op.inner_src_port;
	packet->inner_dst_port = op.inner_dst_port;
	packet->base = op.base;
	packet->offset = op.offset;
	packet->resved = op.resved;
	packet->value = op.value;
	packet->mask = op.mask;
	packet->phy_portId = op.phy_portId;
	packet->rule_type = op.rule_type;
	packet->packet_param = op.packet_param;
	packet->controllerId = op.controllerId;
	packet->volumeParam = op.volumeParam;
	memcpy(packet->correlationInfo, &packet->outer_dst_ip, 4);
	memcpy(packet->correlationInfo + 4, &packet->inner_src_ip, 4);
	for(i = 8; i < CORRLEN; i++)
	{
		packet->correlationInfo[i] = 0x00;
	}
	return sizeof(rule_req_t);
}

int main(int argc, char **argv)
{
	options_t options, temp_options;
	rule_req_t rule;
	int send_socket_descriptor = 0;
	struct sockaddr_in send_addr;
	struct in_addr outer_dst_ip, inner_src_ip;
	u_char *buf = NULL;
	int length = 0;
	int count;
	int rc;
	zlog_category_t *c;
	int i = 0;

	rc = zlog_init("zlog.conf");
	if(rc)
	{
		printf("init fail\n");
		return -1;
	}
	c = zlog_get_category("pmu_cat");
	if(!c)
	{
		printf("get cat fail\n");
		zlog_fini();
		return -2;
	}
	//zlog_info(c, "hello ,zlog");
	init_options (&options);
	cmdline_parser (argc, argv, &options);

	if (options.debug)
	{
		printf ("remote: %s\nrule_port: %d\npacket_port: %d\nflow_port: %d\n", inet_ntoa(options.remote), options.rule_port, options.packet_port, options.flow_port);
		printf("outer_dst_ip:%s,mask:%d\n", inet_ntoa (options.outer_dst_ip), options.outer_dst_ip_mask);
		printf("inner_src_ip:%s,mask:%d\n", inet_ntoa (options.inner_src_ip), options.inner_src_ip_mask);
	}
	memset (&send_addr, '0', sizeof (send_addr));
	//bzero(&send_addr, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = options.remote.s_addr;
	send_addr.sin_port = options.rule_port;
	send_socket_descriptor = socket (AF_INET, SOCK_DGRAM, 0);

	memcpy(&temp_options, &options, sizeof(options));
	for(i = 0; i < 1000; i++)
	{
		if(i % (2 << (32 - options.outer_dst_ip_mask - 1)) == 0)
		{
			temp_options.outer_dst_ip.s_addr = options.outer_dst_ip.s_addr;
		}
		if(i % (2 << (32 - options.inner_src_ip_mask - 1)) == 0)
		{
			temp_options.inner_src_ip.s_addr = options.inner_src_ip.s_addr;
		}
		temp_options.outer_dst_ip.s_addr = htonl (ntohl(temp_options.outer_dst_ip.s_addr) + 0x1);
		temp_options.inner_src_ip.s_addr = htonl (ntohl(temp_options.inner_src_ip.s_addr) + 0x1);
		length = rul_add_req (&rule, i, temp_options);
		count = sendto (send_socket_descriptor, &rule, length, 0, (struct sockaddr *) &(send_addr), sizeof (send_addr));
		if(count == -1)
		{
			zlog_info(c, "send fail.sendto return value:%d", count);
		}
		else
		{
			zlog_info(c, "send success.sendto return value:%d", count);
		}
	}

	zlog_fini();
	return 0;
}
