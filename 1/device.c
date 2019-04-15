/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "device.h"

/* array of device driver file descriptors */
static int driver[NUM_DRIVER];
/* variables for memory-mapped driver */
static int mem_dev;
static unsigned long *mem_dev_addr;
static unsigned char *led_addr;

/**
 * open_drivers - open device drivers
 */
int open_drivers() {
        int error_flag = 0;

        if ((driver[0] = open(FPGA_FND_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n", FPGA_FND_DEVICE);
                error_flag = 1;
        }
        if ((driver[1] = open(FPGA_LCD_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n", FPGA_LCD_DEVICE);
                error_flag = 1;
        }
        if ((driver[2] = open(FPGA_DOT_DEVICE, O_WRONLY)) < 0) {
                printf("Device open error : %s\n", FPGA_DOT_DEVICE);
                error_flag = 1;
        }

	if((mem_dev = open(MEMORY_DEVICE, O_RDWR | O_SYNC)) < 0) {
		printf("Device open error : %s\n", MEMORY_DEVICE);
		error_flag = 1;
	} else {
		mem_dev_addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
					MAP_SHARED, mem_dev, LED_BASE_ADDR);
		if (mem_dev_addr == MAP_FAILED) {
			printf("mmap error\n");
			error_flag = 1;
			close(mem_dev);
		}
		led_addr = (void *)mem_dev_addr + LED_ADDR;
	}

        return error_flag;
}

void write_fnd(char *input) {
	write(driver[0], input, 4);
}

void write_lcd(char *input) {
#define MAX_BUFF	32
#define LINE_BUFF	16
	unsigned char string[MAX_BUFF];
	memset(string, ' ', sizeof(string));
	memcpy(string, input, 32);
	write(driver[1], string, MAX_BUFF);
#undef MAX_BUFF
#undef LINE_BUFF
}

void write_dot(char *input) {
	write(driver[2], input, 10);
}

void write_led(char input) {
	*led_addr = input;
}

/**
 * close_drivers - close device drivers
 */
void close_drivers() {
        int i;
        for (i = 0; i < NUM_DRIVER; i++) {
                close(driver[i]);
        }
	close(mem_dev);
}
