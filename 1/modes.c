/*
 * Copyright (c) 2019 Sanggu Han
 */

#include "modes.h"

/**
 * initialize_board - initialize board states
 */
void initialize_board() {
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
