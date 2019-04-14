/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "modes.h"
#include "message.h"
#include "operation.h"

#define MODE_FUNCTION(num, ...) \
	void mode##num(int input)\
	__VA_ARGS__

/* set function pointer array w/ mode functions */
static mode_func_ptr modes[5] = {
	mode1, mode2, mode3, mode4, mode5
};

/**
 * initialize_board - initialize board states
 */
void initialize_board() {
	control_fnd(FND_RESET);
	control_lcd(LCD_RESET, 0);
	control_dot(DOT_RESET);
	control_led(LED_RESET, 0);
}

/**
 * initiate_mode - set initial state for given mode
 */
void initiate_mode(int mode) {
        switch (mode) {
        case 0:
                /*
                 * mode 1 - clock
                 */
		control_fnd(FND_SET_BOARD_TIME);
		control_led(LED_SET, 1 << 7);
                break;
        case 1:
                /*
                 * mode 2 - counter
                 */
		control_led(LED_SET, 1 << 6);
                break;
        case 2:
                /*
                 * mode 3 - text editor
                 */
		control_dot(DOT_PRINT_INPUT_MODE);
                break;
        case 3:
                /*
                 * mode 4 - draw board
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

void run_mode(int mode, int input) {
	modes[mode](input);
}

MODE_FUNCTION(1, {
	switch (input) {
	case SW_1:
		break;
	case SW_2:
		control_fnd(FND_SET_BOARD_TIME);
		break;
	case SW_3:
		control_fnd(FND_ADD_HOUR);
		break;
	case SW_4:
		control_fnd(FND_ADD_MINUTE);
		break;
	default:
		break;
	}
})

MODE_FUNCTION(2, {
	int base;

	switch (input) {
	case SW_1:
		control_fnd(FND_CHANGE_BASE);
		break;
	case SW_2:
		control_fnd(FND_ADD_SQUARE);
		break;
	case SW_3:
		control_fnd(FND_ADD_BASE);
		break;
	case SW_4:
		control_fnd(FND_INCREASE);
		break;
	default:
		break;
	}

	base = get_fnd_base();
	control_led(LED_SET, 1 << (8 - base));
})

MODE_FUNCTION(3, {
	control_fnd(FND_INCREASE);

	switch (input) {
	case SW_1:
		break;
	case SW_2:
		break;
	case SW_3:
		break;
	case SW_4:
		break;
	case SW_5:
		break;
	case SW_6:
		break;
	case SW_7:
		break;
	case SW_8:
		break;
	case SW_9:
		break;
	case SW_2 | SW_3:
		control_lcd(LCD_RESET, 0);
		break;
	case SW_5 | SW_6:
		control_dot(DOT_PRINT_INPUT_MODE);
		break;
	case SW_8 | SW_9:
		control_lcd(LCD_ADD_CHAR, ' ');
		break;
	default:
		break;
	}
})

MODE_FUNCTION(4, {
	/* variable for loop counter */
	int i;

	control_fnd(FND_INCREASE);

	switch (input) {
	case SW_1:
	case SW_2:
	case SW_3:
	case SW_4:
	case SW_5:
	case SW_6:
	case SW_7:
	case SW_8:
	case SW_9:
		for (i = 0; i < 9; i++) {
			if ((input >> i) & 1) {
				control_dot(i);
			}
		}
		break;
	default:
		break;
	}
})

MODE_FUNCTION(5, {
	switch (input) {
	default:
		break;
	}
})
