/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "process.h"
#include "globals.h"
#include "message.h"
#include "device.h"

#define QUEUE_PERMS 0644
#define BUFF_SIZE 64

/**
 * input_process - get input from board, then pass key to main process
 */
void input_process()
{
	/* Variables for loop counter */
	int i, j;

	/* Variable for message queue */
	int msgqid;

	/* Variables for switch input (BACK, PROG, ...) */
	struct input_event ev[BUFF_SIZE];
	int fd, rd, value, flags, size = sizeof (struct input_event);
	char *device = "/dev/input/event0";
	char msg[256];

	/* Variables for switch input (SW1-9) */
	int swfd, swflag, pressed;
	unsigned char push_sw_buff[9];
	unsigned char pushed[9];
	int buff_size = sizeof(push_sw_buff);
	char *sw = "/dev/fpga_push_switch";

        if ((msgqid = get_message_qid()) == -1) {
                perror("(I) Error occurred while get message queue");
        }

	if((fd = open (device, O_RDONLY)) == -1) {
		printf("%s is not a valid device\n", device);
	}
	/* for non-blocking read() */
	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);


	if((swfd = open(sw, O_RDWR)) == -1) {
		printf("%s is not a valid device\n", device);
	}

	while (1){
		/* set pressed as release state */
		pressed = KEY_RELEASE;
		/* initialize switch state tracker */
		memset(pushed, 0, sizeof(pushed));

		do {
			swflag = 0;
			read(swfd, &push_sw_buff, buff_size);
			for (i = 0; i < 9; i++) {
				swflag |= push_sw_buff[i];
				pushed[i] |= push_sw_buff[i];
			}
			pressed = swflag ? KEY_PRESS : pressed;
			usleep(10000);
		} while (swflag); /* read input until switch is released */

                /* when switch has released, enqueue input */
		if (pressed) {
                        memset(msg, 0, sizeof(msg));
                        /* set switch flag & input */
                        msg[FLAG_SWITCH] = 1;
                        for (i = 0; i < 9; i++) {
                                msg[i] = pushed[i];
                        }
			if (enqueue_message(msgqid, (long)INPUT, msg)) {
                                printf("(I) Failed to enqueue switch input\n");
                        }
		}

		if ((rd = read(fd, ev, size * BUFF_SIZE)) < size) {
			continue;
		}

		value = ev[0].value;

                /* when key has released, enqueue input */
		if (value == KEY_RELEASE) {
                        memset(msg, 0, sizeof(msg));
                        /* set key flag & input */
                        msg[FLAG_ENV] = 1;
			switch(ev[0].code) {
			case KEY_PROG:
			case KEY_BACK:
			case KEY_VOLUMEUP:
			case KEY_VOLUMEDOWN:
                                msg[KEY_ENV] = ev[0].code;
				break;
			default:
				break;
			}
			if (enqueue_message(msgqid, (long)INPUT, msg)) {
                                printf("(I) Failed to enqueue env key input\n");
                        }
		}
		usleep(10000);
	}

	close(fd);
	close(swfd);
}

/**
 * output_process - get results from main process, then print to board
 */
void output_process() {
        int msgqid;

        /* get message queue id for output_process <-> main_process */
        if ((msgqid = get_message_qid()) == -1) {
                perror("(O) Error occurred while get message queue");
        }

        do {
                msg_t message;
                if (receive_message(msgqid, (long)OUTPUT, &message)) {
                        printf("(O) Failed to receive message\n");
                }

                /* TODO: print result properly in here */
        } while(1);
};
