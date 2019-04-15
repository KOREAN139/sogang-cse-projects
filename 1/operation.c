/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "operation.h"
#include "message.h"

/* variables for fnd*/
static unsigned char fnd_array[4];
static int fnd_curr;			/* for mode 2 */
static int fnd_base;			/* for mode 2 */
static int b;				/* for mode 2 */
/* variables for lcd */
static unsigned char lcd_array[8];
static int lcd_idx;
/* variables for dot matrix */
static unsigned char dot_array[10];
static int dx[] = {1, -1};		/* for cursor move */
static int dy[] = {1, -1};		/* for cursor move */
static int cx, cy;			/* for trace cursor */
static int dot_input_mode;
static unsigned char dot_char[][10] = {
	{0x1c, 0x36, 0x63, 0x63, 0x63, 0x7f, 0x7f, 0x63, 0x63, 0x63},	/* A */
	{0x0c, 0x1c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f, 0x3f}	/* 1 */
};
/* variable for led */
static unsigned char led_mask[1];

static void update_base() {
	if (fnd_base == 2) {
		b = 10;
	} else if (fnd_base == 1) {
		b = 2;
	} else {
		b = fnd_base - 3 ? 4 : 8;
	}
}

int get_fnd_base() {
	return fnd_base;
}

void control_fnd(int op) {
	/* variables for digit->string convert loop */
	int i, cur;
	/* variables for board time */
	int h, m;
	time_t t;
	struct tm tm;
	/* variable for message queue */
	int qid;

	switch (op) {
	case FND_INCREASE:
		fnd_curr = (fnd_curr + 1) % (b * b * b * b);
		break;
	case FND_RESET:
		fnd_curr = 0;
		fnd_base = 2;
		update_base();
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
		h = fnd_curr / 100 + !m;
		fnd_curr = h % 24 * 100 + m;
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
		update_base();
		fnd_curr %= b * b * b * b;
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

	/* send current state to ouput_process */
	if((qid = get_message_qid()) == -1) {
		printf("(FND) Error occurred while get message queue");
	}

	if(enqueue_message(qid, (long)OUTPUT, DATA_FND, fnd_array)) {
		printf("(FND) Failed to enqueue switch input\n");
	}
}

void control_lcd(int op, char ch) {
#define MAX_WIDTH	8
	/* variable for loop counter */
	int i;
	/* variable for message queue */
	int qid;

	switch (op) {
	case LCD_RESET:
		lcd_idx = 0;
		memset(lcd_array, ' ', sizeof(lcd_array));
		break;
	case LCD_ADD_CHAR:
		lcd_idx += 1;
		if (lcd_idx > MAX_WIDTH) {
			for (i = 1; i < MAX_WIDTH; i++) {
				lcd_array[i-1] = lcd_array[i];
			}
			lcd_idx = MAX_WIDTH;
		}
	case LCD_REPLACE:
		lcd_array[lcd_idx - 1] = ch;
		break;
	default:
		break;
	}

	/* send current state to ouput_process */
	if((qid = get_message_qid()) == -1) {
		printf("(LCD) Error occurred while get message queue");
	}

	if(enqueue_message(qid, (long)OUTPUT, DATA_LCD, lcd_array)) {
		printf("(LCD) Failed to enqueue switch input\n");
	}
#undef MAX_WIDTH
}

void control_dot(int op) {
#define MAT_WIDTH	7
#define MAT_HEIGHT	10
	/* variable for loop counter */
	int i;
	/* variable for temporary use */
	int t;
	/* variable for message queue */
	int qid;

	switch (op) {
	case DOT_CURSOR_UP:
		cy = (cy + dy[1] + MAT_HEIGHT) % MAT_HEIGHT;
		break;
	case DOT_CURSOR_DOWN:
		cy = (cy + dy[0] + MAT_HEIGHT) % MAT_HEIGHT;
		break;
	case DOT_CURSOR_LEFT:
		cx = (cx + dx[1] + MAT_WIDTH) % MAT_WIDTH;
		break;
	case DOT_CURSOR_RIGHT:
		cx = (cx + dx[0] + MAT_WIDTH) % MAT_WIDTH;
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
		dot_array[cy] ^= 1 << (6 - cx);
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

	/* send current state to ouput_process */
	if((qid = get_message_qid()) == -1) {
		printf("(DOT) Error occurred while get message queue");
	}

	if(enqueue_message(qid, (long)OUTPUT, DATA_DOT, dot_array)) {
		printf("(DOT) Failed to enqueue switch input\n");
	}
#undef MAT_WIDTH
#undef MAT_HEIGHT
}

void control_led(int op, unsigned char mask) {
	/* variable for message queue */
	int qid;

	switch (op) {
	case LED_RESET:
		led_mask[0] = 0;
		break;
	case LED_SET:
		led_mask[0] = mask;
		break;
	default:
		break;
	}

	/* send current state to ouput_process */
	if((qid = get_message_qid()) == -1) {
		printf("(LED) Error occurred while get message queue");
	}

	if(enqueue_message(qid, (long)OUTPUT, DATA_LED, led_mask)) {
		printf("(LED) Failed to enqueue switch input\n");
	}
}
