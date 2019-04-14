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

int enqueue_message(int qid, long mtype, char *msg) {
        msg_t message;
        message.mtype = mtype;
        strcpy(message.msg, msg);
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
