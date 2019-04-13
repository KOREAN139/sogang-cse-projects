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
        char msg[111];
} msg_t;

/*
 * Message types
 */
#define INPUT		1
#define OUTPUT		2

/*
 * Keys
 */
#define KEY_RELEASE	0
#define KEY_PRESS	1
#define KEY_VOLUMEDOWN	114	/* Change mode (inverse) */
#define KEY_VOLUMEUP	115	/* Change mode */
#define KEY_PROG	116
#define KEY_BACK	158	/* Terminates program */

#endif
