/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "globals.h"
#include "process.h"
#include "modes.h"
#include "message.h"
#include "device.h"

/**
 * main - initialize message queue, gets INPUT type message from input_process
 * do proper job w/ key input & current mode, then passes result to
 * output_process
 */
int main(int argc, char *argv[]) {
	/* variable for message queue */
        int msgqid;
	/* variables for process id */
	int input_pid, output_pid;
	/* variables for mode */
        int mode = 0;
	int input;
	/* variable for loop counter */
	int i;

        /* fork input process */
        switch (input_pid = fork()) {
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
        switch (output_pid = fork()) {
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

        /* initialize message queue */
        if ((msgqid = get_message_qid()) == -1) {
                perror("Error occurred while create message queue");
        }

        /* open device drivers */
        if (open_drivers()) {
                goto TERM;
        }

	initialize_board();
	initiate_mode(mode);

        do {
                msg_t message;
                if (receive_message(msgqid, (long)INPUT, &message)) {
                        printf("(M) Failed to receive message\n");
                }

                if (message.msg[FLAG_SWITCH]) {
			input = 0;
			for (i = 0; i < 9; i++) {
				input |= (message.msg[i] << i);
			}
			run_mode(mode, input);
                }

                if (message.msg[FLAG_ENV]) {
                        switch (message.msg[KEY_ENV]) {
                        case KEY_PROG:
                                break;
                        case KEY_VOLUMEUP:
                        case KEY_VOLUMEDOWN:
                                initialize_board();
                                mode += message.msg[KEY_ENV] - KEY_VOLUMEDOWN ? 1 : -1;
                                mode = (mode + 5) % 5;
                                initiate_mode(mode);
                                break;
                        case KEY_BACK:
			        kill(input_pid, SIGKILL);
			        kill(output_pid, SIGKILL);
                                goto TERM;
                        default:
                                break;
                        }
		}
        } while(1);

TERM:
        msgctl(msgqid, IPC_RMID, (struct msqid_ds *)NULL);
	initialize_board();
        close_drivers();

        return 0;
}
