/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "modes.h"
#include "device.h"

/* variables for fnd*/
static unsigned char fnd_array[4];
static int fnd_curr;			/* for mode 2 */
static int fnd_base = 2;		/* for mode 2 */
/* variables for dot matrix */
static unsigned char dot_array[10];
static int dx[] = {0, 0, -1, 1};	/* for cursor move */
static int dy[] = {1, -1, 0, 0};	/* for cursor move */
static int cx, cy;			/* for trace cursor */
static int dot_input_mode;
static unsigned char dot_char[][10] = {
	{0x1c, 0x36, 0x63, 0x63, 0x63, 0x7f, 0x7f, 0x63, 0x63, 0x63},	/* A */
	{0x0c, 0x1c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x3f}	/* 1 */
};

/**
 * initialize_board - initialize board states
 */
void initialize_board() {
	control_fnd(FND_RESET);
	write_fnd(fnd_array);
	write_lcd((char *)0, 0);
	control_dot(DOT_RESET);
	write_dot(dot_array);
}

/**
 * initiate_mode - set initial state for given mode
 */
void initiate_mode(int mode) {
        switch (mode) {
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
	/* variables for digit->string convert loop */
	int i, cur, b;
	/* variables for board time */
	int h, m;
	time_t t;
	struct tm tm;

	if (fnd_base == 2) {
		b = 10;
	} else if (fnd_base == 1) {
		b = 2;
	} else {
		b = fnd_base - 3 ? 4 : 8;
	}

	switch (op) {
	case FND_INCREASE:
		fnd_curr = (fnd_curr + 1) % (b * b * b * b);
		break;
	case FND_RESET:
		fnd_curr = 0;
		fnd_base = 2;
		memset(fnd_array, 0, sizeof(fnd_array));
		break;
	case FND_SET_BOARD_TIME:
		t = time(NULL);
		tm = *localtime(&t);
		fnd_curr = tm.tm_hour * 100 + tm.tm_min;
		break;
	case FND_ADD_MINUTE:
		m = fnd_curr % 100;
		m = (m + 1) % 60;
		fnd_curr = fnd_curr / 100 * 100 + m;
		break;
	case FND_ADD_HOUR:
		h = fnd_curr / 100;
		h = (h + 1) % 24;
		fnd_curr = h * 100 + fnd_curr % 100;
		break;
	case FND_ADD_BASE:
		fnd_curr = (fnd_curr + b) % (b * b * b * b);
		break;
	case FND_ADD_SQUARE:
		fnd_curr = (fnd_curr + b * b) % (b * b * b * b);
		break;
	case FND_CHANGE_BASE:
		fnd_base += fnd_base - 4 ? 1 : -3;
		break;
	default:
		break;
	}

	if (op != FND_RESET) {
		cur = fnd_curr;
		/* convert digit to string */
		for (i = 0; i < 4; i++) {
			fnd_array[3-i] = cur % b;
			cur /= b;
		}
	}
}

void control_dot(int op) {
#define MAT_WIDTH	7
#define MAT_HEIGHT	10
	/* variable for loop counter */
	int i;

	switch (op) {
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
