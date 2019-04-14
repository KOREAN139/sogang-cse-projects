/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/**
 * struct msg_t - used by processes to communicate w/ message queue
 * @mtype: type for current message
 * @msg: data which current message contains
 */
typedef struct msg_t {
        long mtype;
        char msg[256];
} msg_t;

/*
 * Message types
 */
#define INPUT		1
#define OUTPUT		2

/*
 * Keys and Key types
 */
#define KEY_RELEASE	0
#define KEY_PRESS	1
#define FLAG_SWITCH     10      /* Flag for switch key input */
#define FLAG_ENV        11      /* Flag for env key input (PROG, ..) */
#define KEY_ENV         12      /* Index where env key input is stored */
#define KEY_VOLUMEDOWN	114	/* Change mode (inverse) */
#define KEY_VOLUMEUP	115	/* Change mode */
#define KEY_PROG	116
#define KEY_BACK	158	/* Terminates program */

#endif
