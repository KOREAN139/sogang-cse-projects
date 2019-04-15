/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_PERMS 0644
#define MSG_LEN 256

/*
 * Message types
 */
#define INPUT		1
#define OUTPUT		2

/*
 * Data types
 */
#define DATA_INPUT	0
#define DATA_FND	1
#define DATA_LCD	2
#define DATA_DOT	4
#define DATA_LED	8
#define DATA_TERM	16

/**
 * struct msg_t - used by processes to communicate w/ message queue
 * @mtype: type for current message
 * @dtype: type for data which current message contains
 * @msg: data which current message contains
 */
typedef struct msg_t {
        long mtype;
	int dtype;
        char msg[MSG_LEN];
} msg_t;

/**
 * get_message_qid - get message queue id
 * returns message queue id which this program uses
 */
int get_message_qid();
/**
 * enqueue_message - enqueue message with given data
 * @qid: queue id of message queue
 * @mtype: type for message
 * @dtype: type for data which current message contains
 * @msg: data which message will contain
 * return 0 if enqueue was successful, -1 otherwise
 */
int enqueue_message(int qid, long mtype, int dtype, char *msg);
/**
 * receive_message - receive message with given mtype
 * @qid: queue id of message queue
 * @mtype: type for message
 * @msg: pointer to store receive message
 * return 0 if enqueue was successful, -1 otherwise
 */
int receive_message(int qid, long mtype, msg_t *msg);

#endif
