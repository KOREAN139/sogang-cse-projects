/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "globals.h"
#include "process.h"
#include "modes.h"

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
        int mode = 0;

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

                if (msg[FLAG_SWITCH]) {
                }

                if (msg[FLAG_ENV]) {
                        switch(msg[KEY_ENV]) {
                        case KEY_PROG:
                                break;
                        case KEY_VOLUMEUP:
                        case KEY_VOLUMEDOWN:
                                initialize_board();
                                mode += msg[KEY_ENV] - KEY_VOLUMEDOWN ? 1 : -1;
                                mode = (mode + 5) % 5;
                                initiate_mode(mode);
                                break;
                        case KEY_BACK:
			        kill(input_pid, SIGTERM);
			        kill(output_pid, SIGTERM);
                                goto TERM;
                        default:
                                break;
                        }
		}

		/*
                message.mtype = OUTPUT;
                if (msgsnd(msgqid, &message, sizeof(msg_t)-sizeof(long), 0) == -1) {
                        perror("(I) Error occurred msgsnd()");
                }
		*/
        } while(1);

TERM:
        msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);

        return 0;
}

