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
int rul_add_req (rule_req_t *packet, uint32_t seq, options_t op)
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
int add_msg_queue(queue_t *queue, uint32_t seq, int msg_type)
{
	int subscript = 0;
	queue_body_t *msg;
	queue_head_t *temp_head = NULL;;

	msg = (queue_body_t *)malloc (sizeof(queue_body_t));
	if(msg == NULL)
	{
		return FAIL;
	}
	msg->next = NULL;
	msg->seq = seq;
	msg->msg_type = msg_type;
	subscript = seq % (HASH_VALUE);

	/* lock the list */
	pthread_mutex_lock (&(queue->queue_hash[subscript].lock));
	temp_head = queue->queue_hash + subscript;

	msg->next = temp_head->head;
	temp_head->head = msg;
	queue->count ++;
	
	pthread_mutex_unlock (&(temp_head->lock));
    temp_head = NULL;
	/* release the lock*/
	return SUCCESS;
}

/* delete a message from queue*/
int del_msg_queue (queue_t *queue, uint32_t seq)
{
	int subscript = 0;
	queue_head_t *temp_head = NULL;;
	queue_body_t *p = NULL, *p1 = NULL;

	subscript = seq % (HASH_VALUE);

	/* lock the list */
	pthread_mutex_lock (&(queue->queue_hash[subscript].lock));
	if((queue->queue_hash[subscript].head) == NULL)
	{
		pthread_mutex_unlock (&(queue->queue_hash[subscript].lock));
		return FAIL;
	}
	temp_head = queue->queue_hash + subscript;

	p = temp_head->head;
	p1 = p->next;
	if(p->seq == seq)
	{
		temp_head->head = p->next;
		p->next = NULL;
		free (p);
		queue->count --;
		pthread_mutex_unlock (&(queue->queue_hash[subscript].lock));
		return SUCCESS;
	}
	else
	{
		for(;p1 != NULL;p1 = p1->next, p = p->next)
		{
			if(p1->seq == seq)
			{
				p->next = p1->next;
				p1->next = NULL;
				free (p1);
				queue->count --;
				pthread_mutex_unlock (&(queue->queue_hash[subscript].lock));
				return SUCCESS;
			}
		}
	}

	pthread_mutex_unlock (&(temp_head->lock));
    temp_head = NULL;
	/* release the lock*/
	return FAIL;
}
