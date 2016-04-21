#include <pthread.h>

#define CORRLEN 16 
#define QUEUE_SIZE 65535

typedef enum
{
	SUCCESS = 0,
	FAIL = -1,
}return_value_t;

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

typedef enum {
	RULE_PORT = 0x0100,
	RULE_COUNT,
	PACKET_PORT,
	FLOW_PORT,
	MSG_TYPE,
	GRP_ID,
	MSG_VERSION,
	OUTER_SRC_IP,
	OUTER_DST_IP,
	INNER_SRC_IP,
	INNER_DST_IP,
	OUTER_SRC_PORT,
	OUTER_DST_PORT,
	INNER_SRC_PORT,
	INNER_DST_PORT,
	BASE,
	OFFSET,
	RESVED,
	VALUE,
	MASK,
	PHY_PORTID,
	RULE_TYPE,
	PACKET_PARAM,
	CONTROLLERID,
	VOLUMEPARAM,
}getopt_long_t;

#pragma pack(1)
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
#pragma pack(4)

typedef struct queue_body
{
	int seq;
	int msg_type;
	struct queue_body *next;
}queue_body_t;

typedef struct queue_head
{
	pthread_mutex_t lock;
	queue_body_t *head;
}queue_head_t;

typedef struct queue
{
	queue_head_t queue_hash[QUEUE_SIZE];
	int count;
}queue_t;

