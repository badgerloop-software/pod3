#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "can.h"
#include "adc.h"
#include "current_sense.h"
#include "pin_alias.h"

#define BLINK_INTERVAL	100

////////////////FILL_GPIO( CURR_SENSE, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR)
const int board_type = DEV;

inline void printPrompt(void) {
	fputs("[dev-build] $ ", stdout);
	fflush(stdout);
}

int dev_init(void) {

	/* dev specific initializations */
	adc_init();
	current_sense_init();
	adc_start();

	return 0;
}

int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(DEV) || dev_init())
		fault();
	
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
