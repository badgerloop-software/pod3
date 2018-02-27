#include "gpio.h"
#include "board.h"
#include "usart.h"

int io_init(void) {

	int ret = 0;

	/* Initialize LED */
	ret += gpio_setClock(LED_PORT, true);
	ret += gpio_setMode(LED_PORT, LED3_PIN, OUTPUT);
	ret += gpio_setSpeed(LED_PORT, LED3_PIN, LOW_SPEED);
	ret += gpio_writePin(LED_PORT, LED3_PIN, 0);

	/* initialize PA2 and PA15 for USART2 (USB) */
	ret += gpio_setClock(GPIOA, true);
	ret += gpio_setMode(GPIOA, 2, ALT);
	ret += gpio_setSpeed(GPIOA, 2, MEDIUM_SPEED);
	ret += gpio_setAlternateFunc(GPIOA, 2, 7);
	ret += gpio_setMode(GPIOA, 15, ALT);
	ret += gpio_setSpeed(GPIOA, 15, MEDIUM_SPEED);
	ret += gpio_setAlternateFunc(GPIOA, 15, 3);

	return ret;
}

int periph_init(void) {

	int ret = 0;
	uint32_t init_regs[3] = {0, 0, 0};

	init_regs[0] = USART_CR1_RXNEIE;
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);

	return ret;
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

inline void fault(void) {
	while (1) {
		blink_handler(750);
	}
}

inline void set_led(bool state) {
	(state) ?
		gpio_writePin(LED_PORT, LED3_PIN, 1) :
		gpio_writePin(LED_PORT, LED3_PIN, 0);
}
