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

/**
 * struct msg_t - used by processes to communicate w/ message queue
 * @mtype: type for current message
 * @msg: data which current message contains
 */
typedef struct msg_t {
        long mtype;
        char msg[MSG_LEN];
} msg_t;

/**
 * get_message_qid - get message queue id
 */
int get_message_qid();
/**
 * enqueue_message - enqueue message with given data
 * @qid: queue id of message queue
 * @mtype: type for message
 * @msg: data which message will contain
 */
int enqueue_message(int qid, long mtype, char *msg);
/**
 * receive_message - receive message with given mtype
 * @qid: queue id of message queue
 * @mtype: type for message
 * @msg: pointer to store receive message
 */
int receive_message(int qid, long mtype, msg_t *msg);

#endif
