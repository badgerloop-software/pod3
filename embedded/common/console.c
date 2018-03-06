#include <stdio.h>

#include "usart.h"
#include "pcbuffer.h"

char buffer[USART_BUF];

extern void printPrompt(void);
void check_input(void) {
	if (pc_buffer_messageAvailable(USB_RX)) {
		pc_buffer_getMessage(USB_RX, buffer, USART_BUF);
		if (buffer[0] != '\0')
			printf("%s\r\n", buffer);
		printPrompt();
	}
}
