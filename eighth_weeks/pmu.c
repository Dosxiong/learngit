#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/socket.h>

#define CORRLEN 16 

typedef enum
{
	RUL_ADD_REQ = 0x01,
	RUL_ADD_ACK,
	RUL_DEL_REQ,
	RUL_DEL_ACK,
	RUL_QRY_REQ,
	RUL_QRY_ACK,
	RUL_CLR_REQ,
	RUL_CLR_ACK,
	RUL_STA_REQ,
	RUL_STA_ACK,

	KEEP_ALIVE_ACK = 0x11,
	KEEP_ALIVE_REQ = 0x10,

	PRC_PKG_ACK = 0xf1,
	PRC_PKG_REQ = 0xf0,
	SNT_PKG_ACK = 0xf3,
	SNT_PKG_REQ = 0xf2,
	SYS_REST_ACK = 0xf5,
	SYS_REST_REQ = 0xf4,

	F_PRC_PKG_ACK = 0xe1,
	F_PRC_PKG_REQ = 0xe0,
	F_EVT_RPT_ACK = 0xe5,
	F_EVT_RPT_REQ = 0xe4,


	RULE_TYPE_MAX = 0xff,
} tcu_msg_t;


typedef struct rule_req_s
{
	uint8_t msg_type;
	uint8_t grp_id : 4;
	uint8_t msg_version:4;
	uint16_t msg_len;
	uint32_t seqs;
	uint32_t outer_src_ip;
	uint32_t outer_dst_ip;
	uint32_t inner_src_ip;
	uint32_t inner_dst_ip;
	uint16_t outer_src_port;
	uint16_t outer_dst_port;
	uint16_t inner_src_port;
	uint16_t inner_dst_port;
	uint8_t base;
	uint16_t offset;
	uint8_t resved;
	uint32_t value;
	uint32_t mask;	
	uint8_t phy_portId;
	uint8_t  rule_type;
	uint8_t packet_param;
	uint8_t controllerId;
	uint32_t volumeParam;
	//uint16_t rsvd;
	uint8_t correlationInfo[CORRLEN];
} rule_req_t;

typedef struct options
{
	struct in_addr remote;
	int rule_port;
	int rule_count;
	int packet_port;
	int flow_port;
	int debug;
}options_t;

/* init the options */
void init_options(options_t *op)
{
	memset(&(op->remote), '0', sizeof(op->remote));
	op->rule_port = 3789;				
	op->rule_cout = 0;
	op->packet_port = 3799;
	op->flow_port = 3800;
	op->debug = 0;
}

int cmdline_parser (int argc, char **argv, options_t *op)
{
	struct option long_options[] =
	{
		{"debug", 0, NULL, 'd'},
		{"remote", 0, NULL, 'r'},
		{"rule_port", 0, NULL, 0},
		{"rule_count", 0, NULL, 0},
		{"packet_port", 0, NULL, 0},
		{"flow_port", 0, NULL, 0}
	};
	int option_index = 0;
	int c = 0;
	while(1)
	{
		c = getopt_long (argc, argv, "r:d", long_options, &option_index);

		if (c == -1) break;			/* all options have been processed */

		switch(c)
		{
			case 'd':
				op->debug = 1;
				break;
			case 'r':				/* remote host */
				op->remote.s_addr = inet_addr(optarg);
				break;
			case '?':
				break;
			case 0:					/* long option with no short option*/
#if 0
				printf("\nopt:%c\n", c);
				printf("optarg = %s\n", optarg);
				printf("optind = %d\n", optind);
				printf("argv[optind -  1]= %s\n", argv[optind]);
				printf("option_index = %d\n", option_index);
#endif
				/* rule port of the remote host*/
				if(strcmp (long_options[option_index].name, "rule_port") == 0)
				{
					op->rule_port = atoi (argv[optind]);
				}
				/* rule count */
				if(strcmp (long_options[option_index].name, "rule_count") == 0)
				{
					op->rule_port = atoi (argv[optind]);
				}
				/* packet port of the remote host*/
				if(strcmp (long_options[option_index].name, "packet_port") == 0)
				{
					op->packet_port = atoi (argv[optind]);
				}
				/* flow port of the remote host*/
				if(strcmp (long_options[option_index].name, "flow_port") == 0)
				{
					op->flow_port = atoi (argv[optind]);
				}
				break;
			default :
				break;
		}
	}
}

int rul_add_req (char *buf)
{
	rule_req_t packet;
	int i = 0;

	packet.msg_type = RUL_ADD_REQ; 
	packet.msg_version = 0x4;
	packet.grp_id = 0x0;
	packet.msg_len = htons (sizeof(packet));
	packet.seqs = 0x00000001;
	packet.outer_src_ip = inet_addr ("192.168.8.144");
	packet.outer_dst_ip = inet_addr ("192.168.8.34");
	packet.inner_src_ip = inet_addr ("10.0.0.1");
	packet.inner_dst_ip = inet_addr ("180.97.33.107");
	packet.outer_src_port = 0x1235;
	packet.outer_dst_port = 0x1235;
	packet.inner_src_port = 0x7070;
	packet.inner_dst_port = 0x7070;
	packet.base = 0x00;
	packet.offset = 0x0000;
	packet.resver = 0x00;
	packet.value = 0x00000000;
	packet.mask = 0x00000000;
	packet.phy_portId = 0x01;
	packet.rule_type = 0x01;
	packet.packet_param = 0xff;
	packet.controllerId = 0x00;
	packet.volumeParam = 0x00000000;
	for(i = 0; i < 16; i++)
	{
		packet.correlationInfo[i] = 0x00;
	}
}

int main(int argc, char **argv)
{
	options_t options;
	int send_socket_descriptor = 0;
	struct sockaddr_in send_addr;
	char *buf;

	init_options (&options);
	cmdline_parser (argc, argv, &options);
	
	if (options.debug) printf ("remote: %s\nrule_port: %d\npacket_port: %d\nflow_port: %d\n", inet_ntoa(options.remote), options.rule_port, options.packet_port, options.flow_port);

	memset (&send_addr, '0', sizeof (send_addr));
	send_addr.sin_family = AF_INET;
	memcpy (& (send_addr.sin_addr), & (options.remote), sizeof (send_addr.sin_addr));
	send_addr.sin_port = options.rule_port;

	sendto (send_socket_descriptor, buf, sizeof(buf), 0, (struct sockaddr) &send_addr, sizeof (send_addr));
	return 0;
}
