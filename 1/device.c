/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "device.h"

static int driver[NUM_DRIVER];

/**
 * open_drivers - open device drivers
 */
int open_drivers() {
        int error_flag = 0;

        if ((driver[0] = open(FPGA_FND_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n",FPGA_FND_DEVICE);
                error_flag = 1;
        }
        if ((driver[1] = open(FPGA_LCD_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n",FPGA_LCD_DEVICE);
                error_flag = 1;
        }
        if ((driver[2] = open(FPGA_DOT_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n",FPGA_DOT_DEVICE);
                error_flag = 1;
        }

        return error_flag;
}

void write_fnd(char *input) {
	write(driver[0], &input, sizeof(input));
}

void write_lcd(char *input, int len) {
#define MAX_BUFF	32
#define LINE_BUFF	16
	unsigned char string[MAX_BUFF];
	memset(string, ' ', sizeof(string));
	memcpy(string, input, len);
	write(driver[1], string, MAX_BUFF);
#undef MAX_BUFF
#undef LINE_BUFF
}

void write_dot(char *input) {
	write(driver[2], input, sizeof(input));
}

/**
 * close_drivers - close device drivers
 */
void close_drivers() {
        int i;
        for (i = 0; i < NUM_DRIVER; i++) {
                close(driver[i]);
        }
}
