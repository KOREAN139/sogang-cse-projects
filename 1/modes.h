/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _MODES_H_
#define _MODES_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Flags for SIGALRM
 */
#define FLAG_BLINK		1
#define FLAG_MODE_1		2
#define FLAG_BOARD_TIME		4
#define FLAG_MODE_4		8
#define FLAG_MODE_5		16

/*
 * Bit masking for switch input
 */
#define SW_1	0x00000001
#define SW_2	0x00000002
#define SW_3	0x00000004
#define SW_4	0x00000008
#define SW_5	0x00000010
#define SW_6	0x00000020
#define SW_7	0x00000040
#define SW_8	0x00000080
#define SW_9	0x00000100

/* macro for declaring functions */
#define DECLARE_MODE_FUNCTION(num) \
	void mode##num(int input);
/* typedef for function pointer */
typedef void (*mode_func_ptr)(int);

/**
 * periodic_control - alarm signal handler for periodic output
 * @signo: signal number
 */
void periodic_control(int signo);
/**
 * initialize_board - initialize board states
 */
void initialize_board();
/**
 * initiate_mode - set initial state for mode
 * @mode: mode number to initiate
 */
void initiate_mode(int mode);
/**
 * run_mode - run proper mode function for input
 * @mode: mode number to handle current input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
void run_mode(int mode, int input);
/**
 * mode1 - do task for mode 1 with input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
DECLARE_MODE_FUNCTION(1);
/**
 * mode2 - do task for mode 2 with input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
DECLARE_MODE_FUNCTION(2);
/**
 * mode3 - do task for mode 3 with input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
DECLARE_MODE_FUNCTION(3);
/**
 * mode4 - do task for mode 4 with input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
DECLARE_MODE_FUNCTION(4);
/**
 * mode5 - do task for mode 5 with input
 * @input: switch input bit-masked data,
 *	 if i-th switch has pressed, i-th bit masked as 1
 */
DECLARE_MODE_FUNCTION(5);
/**
 * send_result - send result of task to output process
 * @type: type of device (use defined constant in message.h)
 */
void send_result(int type);

#endif
