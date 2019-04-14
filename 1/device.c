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

/**
 * close_drivers - close device drivers
 */
void close_drivers() {
        int i;
        for (i = 0; i < NUM_DRIVER; i++) {
                close(driver[i]);
        }
}
