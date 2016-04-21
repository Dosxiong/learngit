#include "zlog.h"

#define HASH_VALUE 65535

int rul_add_req (rule_req_t *packet, int seq, options_t op);
int add_msg_queue(queue_t *queue, int seq, int msg_type, zlog_category_t *debug, zlog_category_t *error);
int init_queue(queue_t *queue);
