#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"
#include "message.h"

#define REMOTEHOST "192.168.3.34"
#define DEFAULTIP "10.0.0.0"
#define DEFAULTPORT 7070
#define HELPLENG 28

/* init the options */
void init_options(options_t *op)
{
	op->remote.s_addr = inet_addr(REMOTEHOST);
	op->rule_port = htons (3789);				
	op->rule_count = 0;
	op->packet_port = htons (3799);
	op->flow_port = htons (3800);
	op->msg_type = RUL_ADD_REQ;
	op->msg_version = 0x4;
	op->grp_id = 0x0;
	op->outer_src_ip.s_addr = inet_addr (DEFAULTIP);
	op->outer_dst_ip.s_addr = inet_addr (DEFAULTIP);
	op->outer_dst_ip_mask = 16;
	op->inner_src_ip.s_addr = inet_addr (DEFAULTIP);
	op->inner_dst_ip.s_addr = inet_addr (DEFAULTIP);
	op->inner_src_ip_mask = 16;
	op->outer_src_port = htons (DEFAULTPORT);
	op->outer_dst_port = htons (DEFAULTPORT);
	op->inner_src_port = htons (DEFAULTPORT);
	op->inner_dst_port = htons (DEFAULTPORT);
	op->base = 0x00;
	op->offset = htons(2);
	op->resved = 0x00;
	op->value = htonl (0);
	op->mask = htonl (0);
	op->phy_portId = 0x01;
	op->rule_type = 0x01;
	op->packet_param = 0xff;
	op->controllerId = 0x00;
	op->volumeParam = htonl (0);
	op->debug = 0;
	op->help_give = 0;
}

int cmdline_parser (int argc, char **argv, options_t *op)
{
	int i = 0;
	int ip_flag = 0, mask_flag = 0;
	char *ip_char = NULL, *mask_char = NULL;
	struct option long_options[] =
	{
		{"help", 0, NULL, 'h'},
		{"debug", 0, NULL, 'd'},
		{"remote", 0, NULL, 'r'},
		{"rule_port", 0, NULL, RULE_PORT},
		{"rule_count", 0, NULL, RULE_COUNT},
		{"packet_port", 0, NULL, PACKET_PORT},
		{"msg_type", 0, NULL, MSG_TYPE},
		{"msg_version", 0, NULL, MSG_VERSION},
		{"grp_id", 0, NULL, GRP_ID},
		{"outer_src_ip", 0, NULL, OUTER_SRC_IP},
		{"outer_dst_ip", 0, NULL, OUTER_DST_IP},
		{"inner_src_ip", 0, NULL, INNER_SRC_IP},
		{"inner_dst_ip", 0, NULL, INNER_DST_IP},
		{"outer_src_port", 0, NULL, OUTER_SRC_PORT},
		{"outer_dst_port", 0, NULL, OUTER_DST_PORT},
		{"inner_src_port", 0, NULL, INNER_SRC_PORT},
		{"inner_dst_port", 0, NULL, INNER_DST_PORT},
		{"base", 0, NULL, BASE},
		{"offset", 0, NULL, OFFSET},
		{"resved", 0, NULL, RESVED},
		{"value", 0, NULL, VALUE},
		{"mask", 0, NULL, MASK},
		{"phy_portId", 0, NULL, PHY_PORTID},
		{"rule_type", 0, NULL, RULE_TYPE},
		{"packet_param", 0, NULL, PACKET_PARAM},
		{"controllerId", 0, NULL, CONTROLLERID},
		{"volumeParam", 0, NULL, VOLUMEPARAM},
		{"flow_port", 0, NULL, FLOW_PORT}
	};
	int option_index = 0;
	int c = 0;
	char help[HELPLENG][100] = 
	{
		"\n  help(h)               帮助信息\n",
		"  debug(d)              显示调试信息\n",
		"  remote(r)             远程tcu ip\n",
		"  rule_port             规则端口\n",
		"  rule_count            要发送规则的数目\n",
		"  packet_port           报文端口\n",
		"  flow_port             数据流端口\n",
		"  msg_type              消息类型\n",
		"  msg_version           消息版本号\n",
		"  grp_id                组id\n",
		"  outer_src_ip          outer src ip\n",
		"  outer_dst_ip          outer dst ip\n",
		"  inner_src_ip          inner src ip\n",
		"  inner_dst_ip          inner dst ip\n",
		"  outer_src_port        outer src port\n",
		"  outer_dst_port        outer dst port\n",
		"  inner_src_port        inner src port\n",
		"  inner_dst_port        inner dst port\n",
		"  base                  base\n",
		"  offset                offset\n",
		"  resved                resved\n",
		"  value                 offset value\n",
		"  mask                  mask\n",
		"  phy_portId            phy portid\n",
		"  rule_type             rule type\n",
		"  packet_param          packet param\n",
		"  controllerid          controllerid\n",
		"  volumeparam           volumeparam\n\n"
	};

	while(1)
	{
		c = getopt_long (argc, argv, "r:dh", long_options, &option_index);

		if (c == -1) break;			/* all options have been processed */

		switch(c)
		{
			case 'd':
				op->debug = 1;
				break;
			case 'h':
				op->help_give = 1;
				for(i = 0; i < HELPLENG; i++)
				{
					printf("%s", help[i]);
				}
				//printf("%d\n", strlen (help));
				break;
			case 'r':				/* remote host */
#if 0
				printf("\nopt:%c\n", c);
				printf("optarg = %s\n", optarg);
				printf("optind = %d\n", optind);
				printf("argv[optind -  1]= %s\n", argv[optind]);
				printf("option_index = %d\n", option_index);
#endif
				if(optarg == NULL)
				{
					op->remote.s_addr = inet_addr(argv[optind]);
				}
				else
				{
					op->remote.s_addr = inet_addr(optarg);
				}
				break;
			case '?':
				break;
				/* long option with no short option*/
			case RULE_PORT:					
#if 0
				printf("\nopt:%c\n", c);
				printf("optarg = %s\n", optarg);
				printf("optind = %d\n", optind);
				printf("argv[optind -  1]= %s\n", argv[optind]);
				printf("option_index = %d\n", option_index);
#endif
				/* rule port of the remote host */
					op->rule_port = htons (atoi (argv[optind]));
					break;
			case RULE_COUNT:
				/* rule count */
					op->rule_count = atoi (argv[optind]);
					break;
			case PACKET_PORT:
				/* packet port of the remote host */
					op->packet_port = htons (atoi (argv[optind]));
					break;
			case FLOW_PORT:
				/* flow port of the remote host */
					op->flow_port = htons (atoi (argv[optind]));
					break;
			case MSG_TYPE:
				/* msg_type */
					op->msg_type = atoi (argv[optind]);
					break;
			case GRP_ID:
				/* grp_id */
					op->grp_id = atoi (argv[optind]);
					break;
			case MSG_VERSION:
				/* msg_version */
					op->msg_version = atoi (argv[optind]);
					break;
			case OUTER_SRC_IP:
				/* outer src ip */
					for(i = 0; argv[optind][i] != '\0'; i++)
					{
						if(argv[optind][i] == '/')
						{
							ip_flag = i;
							ip_char = (char *)malloc(ip_flag);
							if(ip_char == NULL)
							{
								continue;
							}
							memcpy (ip_char, argv[optind], i);
							op->outer_src_ip.s_addr = inet_addr (ip_char);
							free (ip_char);
						}
					}
					mask_flag = i;
					mask_char = (char *)malloc((mask_flag - ip_flag - 1));
					if(mask_char == NULL)
					{
						continue;
					}
					memcpy (mask_char, (argv[optind] + ip_flag +1), (mask_flag - ip_flag - 1));
					op->outer_src_ip_mask = atoi (mask_char);
					free (mask_char);
					break;
			case OUTER_DST_IP:
				/* outer dst ip */
					for(i = 0; argv[optind][i] != '\0'; i++)
					{
						if(argv[optind][i] == '/')
						{
							ip_flag = i;
							ip_char = (char *)malloc(ip_flag);
							if(ip_char == NULL)
							{
								continue;
							}
							memcpy (ip_char, argv[optind], i);
							op->outer_dst_ip.s_addr = inet_addr (ip_char);
							free (ip_char);
						}
					}
					mask_flag = i;
					mask_char = (char *)malloc((mask_flag - ip_flag - 1));
					if(mask_char == NULL)
					{
						continue;
					}
					memcpy (mask_char, (argv[optind] + ip_flag +1), (mask_flag - ip_flag - 1));
					op->outer_dst_ip_mask = atoi (mask_char);
					free (mask_char);
					break;
			case INNER_SRC_IP:
				/* inner src ip */
					for(i = 0; argv[optind][i] != '\0'; i++)
					{
						if(argv[optind][i] == '/')
						{
							ip_flag = i;
							ip_char = (char *)malloc(ip_flag);
							if(ip_char == NULL)
							{
								continue;
							}
							memcpy (ip_char, argv[optind], i);
							op->inner_src_ip.s_addr = inet_addr (ip_char);
							free (ip_char);
						}
					}
					mask_flag = i;
					mask_char = (char *)malloc((mask_flag - ip_flag - 1));
					if(mask_char == NULL)
					{
						continue;
					}
					memcpy (mask_char, (argv[optind] + ip_flag +1), (mask_flag - ip_flag - 1));
					op->inner_src_ip_mask = atoi (mask_char);
					free (mask_char);
					break;
			case INNER_DST_IP:
				/* inner dst ip */
					for(i = 0; argv[optind][i] != '\0'; i++)
					{
						if(argv[optind][i] == '/')
						{
							ip_flag = i;
							ip_char = (char *)malloc(ip_flag);
							if(ip_char == NULL)
							{
								continue;
							}
							memcpy (ip_char, argv[optind], i);
							op->inner_dst_ip.s_addr = inet_addr (ip_char);
							free (ip_char);
						}
					}
					mask_flag = i;
					mask_char = (char *)malloc((mask_flag - ip_flag - 1));
					if(mask_char == NULL)
					{
						continue;
					}
					memcpy (mask_char, (argv[optind] + ip_flag +1), (mask_flag - ip_flag - 1));
					op->inner_dst_ip_mask = atoi (mask_char);
					free (mask_char);
					break;
			case OUTER_SRC_PORT:
				/* outer src port */
					op->outer_src_port = htons (atoi (argv[optind]));
					break;
			case OUTER_DST_PORT:
				/* outer dst port */
					op->outer_dst_port = htons (atoi (argv[optind]));
					break;
			case INNER_SRC_PORT:
				/* inner src port */
					op->inner_src_port = htons (atoi (argv[optind]));
					break;
			case INNER_DST_PORT:
				/* inner dst port */
					op->inner_dst_port = htons (atoi (argv[optind]));
					break;
			case BASE:
				/* base */
					op->base = atoi (argv[optind]);
					break;
			case OFFSET:
				/* offset */
					op->offset = htons (atoi (argv[optind]));
					break;
			case RESVED:
				/* resved */
					op->base = atoi (argv[optind]);
					break;
			case VALUE:
				/* value */
					op->value = htonl (atoi (argv[optind]));
					break;
			case MASK:
				/* mask */
					op->mask = htonl (atoi (argv[optind]));
					break;
			case PHY_PORTID:
				/* phy_portId */
					op->phy_portId = atoi (argv[optind]);
					break;
			case RULE_TYPE:
				/* rule_type */
					op->rule_type = atoi (argv[optind]);
					break;
			case PACKET_PARAM:
				/* packet_param */
					op->rule_type = atoi (argv[optind]);
					break;
			case CONTROLLERID:
				/* controllerId */
					op->controllerId = atoi (argv[optind]);
					break;
			case VOLUMEPARAM:
					/* volumeParam */
					op->volumeParam = htonl (atof (argv[optind]));
					break;

			default :
				break;
		}
	}
}

