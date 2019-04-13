/*
 * Copyright (c) 2019 Sanggu Han
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_PERMS 0644

/**
 * struct msg_t - used by processes to communicate w/ message queue
 * @mtype: type for current message
 * @msg: string that current message contains
 */
typedef struct msg_t {
        long mtype;
        char msg[111];
} msg_t;

/*
 * Message types
 */
#define INPUT           1
#define OUTPUT          2

/**
 * input_process - get input from board, then pass key to main process
 */
void input_process() {
        key_t key;
        int msgqid;

        // get message queue id for input_process <-> main_process
        key = ftok(__FILE__, 'Z');
        msgqid = msgget(key, QUEUE_PERMS | IPC_CREAT);
        if (msgqid == -1) {
                perror("(I) Error occurred while get message queue");
        }


        // TODO: read key in here
        msg_t message;
        message.mtype = INPUT;
        strcpy(message.msg, "TEST");
        if (msgsnd(msgqid, &message, sizeof(msg_t)-sizeof(long), 0) == -1) {
                perror("(I) Error occurred msgsnd()");
        }
};

/**
 * output_process - get results from main process, then print to board
 */
void output_process() {
        key_t key;
        int msgqid;

        // get message queue id for output_process <-> main_process
        key = ftok(__FILE__, 'Z');
        msgqid = msgget(key, QUEUE_PERMS | IPC_CREAT);
        if (msgqid == -1) {
                perror("(O) Error occurred while get message queue");
        }

        do {
                msg_t message;
                if (msgrcv(msgqid, &message, sizeof(msg_t)-sizeof(long), OUTPUT, 0) == -1) {
                        perror("(O) Error occurred msgrcv()");
                }
                printf("(O) GOT %s\n", message.msg);

                // TODO: print result properly in here
        } while(1);
};

/**
 * main - initialize message queue, gets INPUT type message from input_process
 * do proper job w/ key input & current mode, then passes result to
 * output_process
 */
int main(int argc, char *argv[]) {
        key_t key;
        int msgqid;

        // initialize message queue
        key = ftok(__FILE__, 'Z');
        msgqid = msgget(key, QUEUE_PERMS | IPC_CREAT);
        if (msgqid == -1) {
                perror("Error occurred while create message queue");
        }

        // fork input process
        switch(fork()) {
        case 0:
                input_process();
                exit(0);
                break;
        case -1:
                perror("Error occurred while fork input process");
                break;
        default:
                break;
        }

        // fork output process
        switch(fork()) {
        case 0:
                output_process();
                exit(0);
                break;
        case -1:
                perror("Error occurred while fork output process");
                break;
        default:
                break;
        }

        do {
                msg_t message;
                if (msgrcv(msgqid, &message, sizeof(msg_t)-sizeof(long), INPUT, 0) == -1) {
                        perror("(O) Error occurred msgrcv()");
                }
                printf("(M) Got %s\n", message.msg);

                //TODO: do proper job in here

                message.mtype = OUTPUT;
                if (msgsnd(msgqid, &message, sizeof(msg_t)-sizeof(long), 0) == -1) {
                        perror("(I) Error occurred msgsnd()");
                }
        } while(1);

        msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);

        return 0;
}
