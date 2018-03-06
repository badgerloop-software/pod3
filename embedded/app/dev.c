#include <stdio.h>
#include "system.h"
#include "board.h"

#include "usart.h"
#include "pcbuffer.h"

#define BLINK_INTERVAL	100

//extern int _write(int fd, const void *buf, size_t count);

int main(void) {

	//char temp[1] = {'a'};

	if (io_init() || periph_init())
		fault();

	while (1) {

		//blink_handler(BLINK_INTERVAL);
		//printf("hello\r\n");
		//_write(0, temp, 1);

		__disable_irq();
		pc_buffer_add(USB_TX, 'h');
		pc_buffer_add(USB_TX, 'e');
		pc_buffer_add(USB_TX, 'l');
		pc_buffer_add(USB_TX, 'l');
		pc_buffer_add(USB_TX, 'o');
		pc_buffer_add(USB_TX, '\r');
		pc_buffer_add(USB_TX, '\n');
		__enable_irq();
		USB_UART->CR1 |= USART_CR1_TXEIE;

		set_led(true);
		delay_ms(500);
		set_led(false);
		delay_ms(500);
	}

	return 0;
}