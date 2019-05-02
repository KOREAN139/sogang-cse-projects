/*
 * Copyright (c) 2019 Sanggu Han
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

/**
 * input_process - get input from board, then pass key to main process
 */
void input_process();
/**
 * output_process - get results from main process, then print to board
 */
void output_process();

#endif
