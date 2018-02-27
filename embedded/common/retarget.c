#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"

#define BLOCK	1

int _write(int fd, const void *buf, size_t count) {
	for (fd = 0; (size_t) fd < count; fd++) {
		if (pc_buffer_full(USB_TX)) {
			USB_UART->CR1 |= USART_CR1_TXEIE;
			while (pc_buffer_full(USB_TX)) {;}
		}
		__disable_irq();
		pc_buffer_add(USB_TX, *((char *) buf++));
		__enable_irq();
	}
	USB_UART->CR1 |= USART_CR1_TXEIE;
#if BLOCK
	while (!pc_buffer_empty(USB_TX)) {;}
#endif
	return count;
}

int _read(int fd, const void *buf, size_t count) {
	for (fd = 0; (size_t) fd < count; fd++) {
#if !BLOCK
		if (pc_buffer_empty(USB_RX))
			return fd;
#else
		while (pc_buffer_empty(USB_RX)) {;}
#endif
		__disable_irq();
		pc_buffer_remove(USB_RX, (char *) buf++);
		__enable_irq();
	}
	return count;
}
