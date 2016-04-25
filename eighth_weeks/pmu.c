#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <math.h>
#include "zlog.h"
#include "message.h"
#include "cmdline.h"

#define MAXMSGSIZE 1024
#define HOST "127.0.0.1"

int s_socket_descriptor = 0;
int r_socket_descriptor = 0;
int rc;
zlog_category_t *debug, *error;
queue_t queue;
options_t options;
int s_count = 0, r_count = 0;

/* the thread of send add rule req */
void *send_add_rule_req()
{
	uint32_t i = 0;
	options_t temp_options;
	rule_req_t rule;
	u_char *buf = NULL;
	int length = 0;
	int count;
	struct sockaddr_in send_addr;

	memset (&send_addr, '0', sizeof (send_addr));
	//bzero(&send_addr, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = options.remote.s_addr;
	send_addr.sin_port = options.rule_port;
	s_socket_descriptor = socket (AF_INET, SOCK_DGRAM, 0);
	memcpy(&temp_options, &options, sizeof(options));

	for(i = 0; i < options.rule_count; i++)
	{
		usleep(100);
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
		count = sendto (s_socket_descriptor, &rule, length, 0, (struct sockaddr *) &(send_addr), sizeof (send_addr));
#if 0
		if(count != -1)
		{
			s_count ++;
		}
#endif
		if(count == -1)
		{
			zlog_info(debug, "send fail.sendto return value:%d", count);
		}
		else
		{
			s_count ++;
			if (add_msg_queue (&queue, i, options.msg_type))
			{
				zlog_info (debug, "add a msg to queue!seq:%d,queue count:%d", i, queue.count);
			}
			else
			{
				zlog_info (error, "alloc fail");
			}
			zlog_info(debug, "send success.sendto return value:%d,seq:%d", count, i);
		}
	}

}

void *count()
{
	while(1)
	{
		sleep(1);
		printf("send count%d,resved count%d\n", s_count, r_count);
	}
}

int main(int argc, char **argv)
{
	pthread_t send_thread, count_thread;
	struct sockaddr_in sin,r_sin;
	int sin_len = 0, r_sin_len = 0;
	char message[MAXMSGSIZE];
	fd_set fds;
	int length;
	struct timeval timeout;
	uint32_t seq = 0;
	rule_rep_t *rep;

	/* init and load log */
	rc = zlog_init("zlog.conf");
	if(rc)
	{
		printf("init fail\n");
		return -1;
	}
	debug = zlog_get_category("pmu_debug");
	if(!debug)
	{
		printf("get debug cat fail\n");
		zlog_fini();
		return -2;
	}
	error = zlog_get_category("pmu_error");
	if(!error)
	{
		printf("get error cat fail\n");
		zlog_fini();
		return -2;
	}

	init_queue (&queue);
	init_options (&options);
	cmdline_parser (argc, argv, &options);

	if(options.help_give)
	{
		return 0;
	}
	timeout.tv_sec = 1;
	timeout.tv_usec = 500000;
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = options.rule_port;
	//sin.sin_port = htons (7070);
	sin.sin_addr.s_addr = htonl (INADDR_ANY);
	sin_len = sizeof(sin);
	bzero (&r_sin, sizeof (r_sin));
	r_sin_len = sizeof (r_sin);
	r_socket_descriptor = socket (AF_INET, SOCK_DGRAM, 0);
	bind(r_socket_descriptor, (struct sockaddr *)&sin, sizeof(sin));

	if (options.debug)
	{
		printf ("remote: %s\nrule_port: %d\npacket_port: %d\nflow_port: %d\n", inet_ntoa(options.remote), options.rule_port, options.packet_port, options.flow_port);
		printf("outer_dst_ip:%s,mask:%d\n", inet_ntoa (options.outer_dst_ip), options.outer_dst_ip_mask);
		printf("inner_src_ip:%s,mask:%d\n", inet_ntoa (options.inner_src_ip), options.inner_src_ip_mask);
		printf("msg_type:%0x\n", options.msg_type);
		printf("grp_id:%0x\n",options.grp_id);
		printf("controllerid:%0x\n",options.controllerId);
	}
	
	pthread_create (&send_thread, NULL, send_add_rule_req, NULL);
	pthread_create (&count_thread, NULL, count, NULL);

	while(1)
	{
		FD_ZERO (&fds);
		FD_SET (r_socket_descriptor, &fds);
		switch (select (r_socket_descriptor + 1, &fds, NULL, NULL, &timeout) > 0)
		{
			case 0:
				break;
			case -1:
				break;
			default:
				break;
		}
		if(FD_ISSET (r_socket_descriptor, &fds))
		{
			length = recvfrom (r_socket_descriptor, message, sizeof(message), 0, (struct sockaddr *)&r_sin, &r_sin_len);
			
#if 0
			if(length != -1)
			{
				r_count ++;
			}
#endif
			if(length != -1)
			{
				r_count ++;
				//seq = atof (message);
				rep = (rule_rep_t *)message;
				if(rep->res_code == 0x00)
				{
					zlog_info (debug, "add rule success!rule id:%u", ntohl(rep->rule_id));
					if(del_msg_queue (&queue, ntohl (rep->seqs)) == FAIL)
					{
						zlog_info (error, "can not find this seq in the queue! seq:%u", seq);
					}
					else
					{
						zlog_info (debug, "find this seq in the queue!seq:%u queue count:%d", seq, queue.count);
					}
				}
				else
				{
					zlog_info (error, "rule add fail");
				}
			}
			//printf("receive msg:%u,count:%d\n", seq, queue.count);
		}

	}

	zlog_fini();
	return 0;
}
