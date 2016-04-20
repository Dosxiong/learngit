#ifndef PMU1_H
#define PMU1_H

#include <arpa/inet.h>

typedef struct options
{
	struct in_addr remote;
	int rule_port;
	int rule_count;
	int packet_port;
	int flow_port;
	uint8_t msg_type;
	uint8_t grp_id : 4;
	uint8_t msg_version:4;
	struct in_addr outer_src_ip;
	struct in_addr outer_dst_ip;
	int outer_dst_ip_mask;
	struct in_addr inner_src_ip;
	struct in_addr inner_dst_ip;
	int inner_src_ip_mask;
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
	int debug;
}options_t;

void init_options(options_t *op);
int cmdline_parser (int argc, char **argv, options_t *op);
#endif
