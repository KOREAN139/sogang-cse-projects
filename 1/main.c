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

#include "globals.h"
#include "process.h"

#define QUEUE_PERMS 0644

/**
 * main - initialize message queue, gets INPUT type message from input_process
 * do proper job w/ key input & current mode, then passes result to
 * output_process
 */
int main(int argc, char *argv[]) {
        key_t key;
        int msgqid;
	int input_pid, output_pid;

        /* initialize message queue */
        key = ftok(__FILE__, 'Z');
        msgqid = msgget(key, QUEUE_PERMS | IPC_CREAT);
        if (msgqid == -1) {
                perror("Error occurred while create message queue");
        }

        /* fork input process */
        switch(input_pid = fork()) {
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

        /* fork output process */
        switch(output_pid = fork()) {
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

		if (!strcmp(message.msg, "BACK")) {
			kill(input_pid, SIGTERM);
			kill(output_pid, SIGTERM);
			break;
		}

                //TODO: do proper job in here

		/*
                message.mtype = OUTPUT;
                if (msgsnd(msgqid, &message, sizeof(msg_t)-sizeof(long), 0) == -1) {
                        perror("(I) Error occurred msgsnd()");
                }
		*/
        } while(1);

        msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);

        return 0;
}

