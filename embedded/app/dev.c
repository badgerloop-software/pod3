#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "can.h"

#define BLINK_INTERVAL	100

inline void printPrompt(void) {
	fputs("[dev-build] $ ", stdout);
	fflush(stdout);
}

int dev_init(void) {

	/* dev specific initializations */

	return 0;
}

int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(DEV) || dev_init())
		fault();
	
	initialize_state_machine(IDLE);
	rx = get_rx(USB_UART);

	post("Developmental");
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
		can_read();
	}

	return 0;
}
