#include "zlog.h"

#define HASH_VALUE 65535

int rul_add_req (rule_req_t *packet, uint32_t seq, options_t op);
int add_msg_queue(queue_t *queue, uint32_t seq, int msg_type);
int init_queue(queue_t *queue);
int del_msg_queue (queue_t *queue, uint32_t seq);
