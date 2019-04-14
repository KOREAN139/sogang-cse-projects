/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "modes.h"
#include "device.h"

/* variables for fnd*/
static unsigned char fnd_array[4];
static int fnd_curr = 0;		/* for mode 2 */
static int fnd_base = 0;		/* for mode 2 */
/* variables for dot matrix */
static unsigned char dot_array[10];
static int dx[] = {0, 0, -1, 1};	/* for cursor move */
static int dy[] = {1, -1, 0, 0};	/* for cursor move */
static int cx = 0, cy = 0;		/* for trace cursor */
static int dot_input_mode = 0;
static unsigned char dot_char[][10] = {
	{0x1c, 0x36, 0x63, 0x63, 0x63, 0x7f, 0x7f, 0x63, 0x63, 0x63},	/* A */
	{0x0c, 0x1c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x3f}	/* 1 */
};

/**
 * initialize_board - initialize board states
 */
void initialize_board() {
	int i;
	for (i = 0; i < 4; i++) {
		fnd_array[i] = 0;
	}
	write_fnd(fnd_array);
	write_lcd((char *)0, 0);
	control_dot(DOT_RESET);
	write_dot(dot_array);
}

/**
 * initiate_mode - set initial state for given mode
 */
void initiate_mode(int mode) {
        switch(mode) {
        case 0:
                /*
                 * mode 1 - clock
                 * set fnd as board time
                 * set led(1) on
                 */
                break;
        case 1:
                /*
                 * mode 2 - counter
                 * set fnd as 0000
                 * set led(2) on
                 */
                break;
        case 2:
                /*
                 * mode 3 - text editor
                 * set fnd as 0000
                 * clear lcd
                 * set dot matrix to print A
                 */
                break;
        case 3:
                /*
                 * mode 4 - draw board
                 * set fnd as 0000
                 * set blinking dot on dot matrix
                 */
                break;
        case 4:
                /*
                 * mode 5 - additional function
                 */
                break;
        default:
                break;
        }
}

void control_fnd(int op) {
	switch(op) {
	// TODO: implement this
	default:
		break;
	}
}

void control_dot(int op) {
#define MAT_WIDTH	7
#define MAT_HEIGHT	10
	int i;

	switch(op) {
	case DOT_CURSOR_UP:
	case DOT_CURSOR_DOWN:
	case DOT_CURSOR_LEFT:
	case DOT_CURSOR_RIGHT:
		cx = (cx + dx[op] + MAT_WIDTH) % MAT_WIDTH;
		cy = (cy + dy[op] + MAT_HEIGHT) % MAT_HEIGHT;
		break;
	case DOT_CURSOR_SHOW:
		// TODO: implement this
		break;
	case DOT_RESET:
		dot_input_mode = 0;
		cx = cy = 0;
	case DOT_CLEAR:
		memset(dot_array, 0, sizeof(dot_array));
		break;
	case DOT_SELECT:
		// TODO: implement this
		break;
	case DOT_REVERSE:
		for (i = 0; i < 10; i++) {
			dot_array[i] ^= 0xff;
		}
		break;
	case DOT_PRINT_INPUT_MODE:
		memcpy(dot_array, dot_char[dot_input_mode], sizeof(dot_array));
		dot_input_mode ^= 1;
		break;
	default:
		break;
	}
#undef MAT_WIDTH
#undef MAT_HEIGHT
}
