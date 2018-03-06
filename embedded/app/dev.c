#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"

#define APP_NAME		"dev"
#define BLINK_INTERVAL	100

inline void printPrompt(void) {
	printf("[%s-build] $ ", APP_NAME);
	fflush(stdout);
}

int dev_init(void) {

	/* dev specific initializations */

	return 0;
}

int main(void) {

	if (io_init() || periph_init() || dev_init())
		fault();

	printf("Program '%s' start\r\n", APP_NAME);
	printPrompt();

	while (1) {
		check_input();
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
