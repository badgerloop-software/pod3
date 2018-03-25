#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "pin_alias.h"

int io_init(void) {

	return gpioAliasInit();
}

int periph_init(void) {

	int ret = 0;
	uint32_t init_regs[3] = {0, 0, 0};

	/* USB UART */
	init_regs[0] = USART_CR1_RXNEIE;
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);

	return ret;
}

#define LED3_PIN	3
#define LED_PORT	GPIOB

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
