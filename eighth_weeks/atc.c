#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "message.h"
#include "cmdline.h"
#include "atc.h"
#include "zlog.h"

/* fill the rule add packet */
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

/* init the queue */
int init_queue(queue_t *queue)
{
	int i = 0;
	
	for(i = 0; i < QUEUE_SIZE; i++)
	{
		pthread_mutex_init (&(queue->queue_hash[i].lock), NULL);
			queue->queue_hash[i].head = NULL;
	}

	queue->count = 0;
	return 1;
}

/* add a message to queue*/
int add_msg_queue(queue_t *queue, int seq, int msg_type, zlog_category_t *debug, zlog_category_t *error)
{
	int subscript = 0;
	queue_body_t *msg;
	queue_head_t *temp_head = NULL;;

	msg = (queue_body_t *)malloc (sizeof(queue_body_t));
	if(msg == NULL)
	{
		zlog_info(error, "alloc queue body fail");
		return FAIL;
	}
	msg->next = NULL;
	msg->seq = seq;
	msg->msg_type = msg_type;
	subscript = seq % HASH_VALUE;

	/* lock the list */
	pthread_mutex_lock (&(queue->queue_hash[subscript].lock));
	temp_head = queue->queue_hash + subscript;

	msg->next = temp_head->head;
	temp_head->head = msg;
	queue->count ++;
	
	pthread_mutex_unlock (&(temp_head->lock));
    temp_head = NULL;
	/* release the lock*/
	zlog_info(debug, "add a msg to queue !seq:%d hash code:%d", seq, subscript);
	return SUCCESS;
}
