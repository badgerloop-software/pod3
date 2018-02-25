#include "gpio.h"
#include "board.h"

void io_init(void) {

	/* Initialize LED */
	gpio_setClock(LED_PORT, true);
	gpio_setMode(LED_PORT, LED3_PIN, OUTPUT);
	gpio_setSpeed(LED_PORT, LED3_PIN, LOW_SPEED);
	gpio_writePin(LED_PORT, LED3_PIN, 0);
}

inline void blink_handler(unsigned int blink_int) {

	static unsigned int curr = 0, prev = 0;

	curr = ticks / blink_int;
	if (curr != prev)
		(curr % 2) ?
			gpio_writePin(LED_PORT, LED3_PIN, 1) :
			gpio_writePin(LED_PORT, LED3_PIN, 0);
	prev = curr;
}
