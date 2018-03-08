#include <stdio.h>

#include "console.h"
#include "usart.h"
#include "pcbuffer.h"

char buffer[USART_BUF];

void check_input(void) {

	PC_Buffer *rx = get_rx(USB_UART);

	if (pc_buffer_messageAvailable(rx)) {

		pc_buffer_getMessage(rx, buffer, USART_BUF);

		if (buffer[0] != '\0')
			printf("%s\r\n", buffer);

		printPrompt();
	}
}
