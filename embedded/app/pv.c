#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"

#define APP_NAME		"pv"
#define BLINK_INTERVAL	250

inline void printPrompt(void) {
	printf("[%s-build] $ ", APP_NAME);
	fflush(stdout);
}

int pv_init(void) {

	/* pv specific initializations */

	return 0;
}

int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init() || pv_init())
		fault();

	rx = get_rx(USB_UART);

	post(APP_NAME);
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
