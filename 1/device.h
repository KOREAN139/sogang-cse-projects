/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define NUM_DRIVER	3
#define FPGA_FND_DEVICE	"/dev/fpga_fnd"
#define FPGA_LCD_DEVICE	"/dev/fpga_text_lcd"
#define FPGA_DOT_DEVICE	"/dev/fpga_dot"

#define MEMORY_DEVICE	"/dev/mem"
#define LED_BASE_ADDR	0x08000000
#define LED_ADDR	0x16

/**
 * open_drivers - open device drivers
 */
int open_drivers();
/**
 * close_drivers - close device drivers
 */
void close_drivers();
/**
 * write_fnd - write input on fnd
 * @input: 4 digits which will displayed on fnd
 */
void write_fnd(char *input);
/**
 * write_lcd - write input on lcd
 * @input: string which will displayed on lcd
 */
void write_lcd(char *input);
/**
 * write_dot - write input on dot matrix
 * @input: 7x10 matrix string (unsigned char[10])
 */
void write_dot(char *input);
/**
 * write_led - write input on led
 * @input: led on-off bit-masked 8bit data
 */
void write_led(char input);

#endif
