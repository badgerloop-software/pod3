#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "can.h"
#include "adc.h"
#include "current_sense.h"
#include "pin_alias.h"
#include "state_machine.h"


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
	initialize_state_machine(IDLE);

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

	unsigned int lastDAQ = 0, lastState = 0, lastTelem = 0, lastHrtbt = 0;
	unsigned int currDAQ = 0, currState = 0, currTelem = 0, currHrtbt = 0;
	while (1) {

		currDAQ = (ticks + 20) / 100;
		currState = (ticks + 40) / 100;
		currTelem = (ticks + 60) / 100;
		currHrtbt = (ticks + 80) / 100;
	
		if( currDAQ != lastDAQ ){
			lastDAQ = currDAQ;
		}	
		if( currState != lastState ){
			state_machine_handler();
			lastState = currState;
		}	
		if( currTelem != lastTelem ){
				
			lastTelem = currTelem;
		}	
		if( currHrtbt != lastHrtbt ){
			
			lastHrtbt = currHrtbt;
		}	
		

		check_input(rx);
		blink_handler(BLINK_INTERVAL);
		can_read();
	}

	return 0;
}
