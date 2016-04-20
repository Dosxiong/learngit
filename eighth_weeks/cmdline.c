#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"

#define REMOTEHOST "192.168.3.34"
#define DEFAULTIP "10.0.0.0/16"

/* init the options */
void init_options(options_t *op)
{
	op->remote.s_addr = inet_addr(REMOTEHOST);
	op->rule_port = htons (3789);				
	op->rule_count = 0;
	op->packet_port = htons (3799);
	op->flow_port = htons (3800);
	op->outer_dst_ip.s_addr = inet_addr (DEFAULTIP);
	op->outer_dst_ip_mask = 0;
	op->inner_src_ip.s_addr = inet_addr (DEFAULTIP);
	op->inner_src_ip_mask = 0;
	op->debug = 0;
}

int cmdline_parser (int argc, char **argv, options_t *op)
{
	int i = 0;
	int ip_flag = 0, mask_flag = 0;
	char *ip_char = NULL, *mask_char = NULL;
	struct option long_options[] =
	{
		{"debug", 0, NULL, 'd'},
		{"remote", 0, NULL, 'r'},
		{"rule_port", 0, NULL, 0},
		{"rule_count", 0, NULL, 0},
		{"packet_port", 0, NULL, 0},
		{"outer_dst_ip", 0, NULL, 0},
		{"inner_src_ip", 0, NULL, 0},
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
			case 0:					/* long option with no short option*/
#if 0
				printf("\nopt:%c\n", c);
				printf("optarg = %s\n", optarg);
				printf("optind = %d\n", optind);
				printf("argv[optind -  1]= %s\n", argv[optind]);
				printf("option_index = %d\n", option_index);
#endif
				/* rule port of the remote host */
				if(strcmp (long_options[option_index].name, "rule_port") == 0)
				{
					op->rule_port = htons (atoi (argv[optind]));
				}
				/* rule count */
				if(strcmp (long_options[option_index].name, "rule_count") == 0)
				{
					op->rule_count = atoi (argv[optind]);
				}
				/* packet port of the remote host */
				if(strcmp (long_options[option_index].name, "packet_port") == 0)
				{
					op->packet_port = htons (atoi (argv[optind]));
				}
				/* flow port of the remote host */
				if(strcmp (long_options[option_index].name, "flow_port") == 0)
				{
					op->flow_port = htons (atoi (argv[optind]));
				}
				/* outer dst ip */
				if(strcmp (long_options[option_index].name, "outer_dst_ip") == 0)
				{
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
				}
				/* inner src ip */
				if(strcmp (long_options[option_index].name, "inner_src_ip") == 0)
				{
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
				}

				break;
			default :
				break;
		}
	}
}

