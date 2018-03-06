#include <stdio.h>

#include "console.h"
#include "usart.h"
#include "pcbuffer.h"

char buffer[USART_BUF];

void check_input(void) {
	if (pc_buffer_messageAvailable(USB_RX)) {
		pc_buffer_getMessage(USB_RX, buffer, USART_BUF);
		if (buffer[0] != '\0')
			printf("%s\r\n", buffer);
		printPrompt();
	}
}
