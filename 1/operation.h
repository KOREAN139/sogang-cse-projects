/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _OPERATION_H_
#define _OPERATIOM_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * Options for fnd operation
 */
#define FND_INCREASE		0
#define FND_SET_BOARD_TIME	1
#define FND_ADD_MINUTE		2
#define FND_ADD_HOUR		3
#define FND_ADD_BASE		4
#define FND_ADD_SQUARE		5
#define FND_CHANGE_BASE		6
#define FND_RESET		7

/*
 * Options for lcd opertaion
 */
#define LCD_RESET		0
#define LCD_ADD_CHAR		1
#define LCD_REPLACE		2

/*
 * Options for dot matrix operation
 */
#define DOT_PRINT_INPUT_MODE	0
#define DOT_RESET		1
#define DOT_CURSOR_UP		2
#define DOT_CURSOR_SHOW		3
#define DOT_CURSOR_LEFT		4
#define DOT_SELECT		5
#define DOT_CURSOR_RIGHT	6
#define DOT_CLEAR		7
#define DOT_CURSOR_DOWN		8
#define DOT_REVERSE		9

/*
 * Options for led operation
 */
#define LED_RESET		0
#define LED_SET			1

/**
 * get_fnd_base - returns current value of fnd_base
 */
int get_fnd_base();
/**
 * control_fnd - control input for fnd on board
 * @op: option which decides operation for this function
 */
void control_fnd(int op);
/**
 * control_lcd - control input for lcd on board
 * @op: option which decides operation for this function
 * @ch: character which append to string or replace last character
 */
void control_lcd(int op, char ch);
/**
 * control_dot - control input for dot matrix on board
 * @op: option which decides operation for this function
 */
void control_dot(int op);
/**
 * control_led - control input for led on board
 * @op: option which decides operation for this function
 * @mask: bit-masked led on-off data
 */
void control_led(int op, unsigned char mask);

#endif
