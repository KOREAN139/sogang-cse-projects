/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM_DRIVER 3
#define FPGA_FND_DEVICE "/dev/fpga_fnd"
#define FPGA_LCD_DEVICE "/dev/fpga_text_lcd"
#define FPGA_DOT_DEVICE "/dev/fpga_dot"

/**
 * open_drivers - open device drivers
 */
int open_drivers();
/**
 * close_drivers - close device drivers
 */
void close_drivers();

#endif
