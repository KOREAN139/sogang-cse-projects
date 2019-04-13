/*
 * Copyright (c) 2019 Sanggu Han
 */

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <stdio.h>

#define BUFF_SIZE	64

/*
 * Keys
 */
#define KEY_RELEASE	0
#define KEY_PRESS	1
#define KEY_PROG	116
#define KEY_BACK	158	/* Terminates program */
#define KEY_VOLUMEDOWN	114	/* Change mode (inverse) */
#define KEY_VOLUMEUP	115	/* Change mode */

int main (int argc, char *argv[])
{
	struct input_event ev[BUFF_SIZE];
	int i, j;
	// Variables for switch input (BACK, PROG, ...)
	int fd, swfd, swflag, rd, value, size = sizeof (struct input_event);
	// Variables for switch input (SW1-9)
	unsigned char push_sw_buff[9];
	unsigned char pushed[9];
	int buff_size = sizeof(push_sw_buff);

	char *device = "/dev/input/event0";
	char *sw = "/dev/fpga_push_switch";

	if((fd = open (device, O_RDONLY)) == -1) {
		printf("%s is not a valid device\n", device);
	}

	if((swfd = open(sw, O_RDWR)) == -1) {
		printf("%s is not a valid device\n", device);
	}

	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	int temp;

	while (1){
		temp = 0;
		for (i = 0; i < 9; i++)
			pushed[i] = 0;

		do {
			swflag = 0;
			read(swfd, &push_sw_buff, buff_size);
			for (i = 0; i < 9; i++) {
				swflag |= push_sw_buff[i];
				pushed[i] |= push_sw_buff[i];
			}
			temp = swflag ? swflag : temp;
			usleep(10000);
		} while (swflag);

		if (temp) {
			for (i = 0; i < 9; i++)
				printf("[%d]", pushed[i]);
			printf("\n");
		}

		if ((rd = read(fd, ev, size * BUFF_SIZE)) < size) {
			continue;
		}

		value = ev[0].value;

		if (value == KEY_RELEASE) {
			switch(ev[0].code) {
			case KEY_PROG:
				printf("PROG\n");
				break;
			case KEY_BACK:
				printf("BACK\n");
				break;
			case KEY_VOLUMEUP:
				printf("VOLUME UP\n");
				break;
			case KEY_VOLUMEDOWN:
				printf("VOLUME DOWN\n");
				break;
			default:
				break;
			}
		}
	}

	close(fd);
	close(swfd);

	return 0;
}
