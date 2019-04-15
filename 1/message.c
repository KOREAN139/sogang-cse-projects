/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "message.h"

int get_message_qid() {
        key_t key;
        int qid;

        key = ftok(__FILE__, 'Z');
        qid = msgget(key, QUEUE_PERMS | IPC_CREAT);

        return qid;
}

int enqueue_message(int qid, long mtype, int dtype, char *msg) {
	/* variable for loop counter */
	int i;
	/* variable for msg length */
	int len;
	/* variable for message queue */
        msg_t message;

	switch (dtype) {
	case DATA_INPUT:
		len = MSG_LEN;
		break;
	case DATA_FND:
		len = 4;
		break;
	case DATA_LCD:
		len = 32;
		break;
	case DATA_DOT:
		len = 10;
		break;
	case DATA_LED:
		len = 1;
		break;
	case DATA_TERM:
		len = 0;
		break;
	default:
		break;
	}

        message.mtype = mtype;
        message.dtype = dtype;

	for (i = 0; i < len; i++) {
		message.msg[i] = msg[i];
	}
        if (msgsnd(qid, &message, sizeof(msg_t)-sizeof(long), 0) == -1) {
                perror("Error occurred msgsnd()");
                return -1;
        }
	return 0;
};

int receive_message(int qid, long mtype, msg_t *msg) {
        if (msgrcv(qid, msg, sizeof(msg_t)-sizeof(long), mtype, 0) == -1) {
                perror("Error occurred msgrcv()");
                return -1;
        }
        return 0;
}
