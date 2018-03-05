#include <stdio.h>
#include "system.h"
#include "board.h"

#include "usart.h"
#include "pcbuffer.h"

#define BLINK_INTERVAL	100

char buffer[USART_BUF];

inline void printPrompt(void) {
	printf("=> ");
	fflush(stdout);
}

void check_input(void) {
	if (pc_buffer_messageAvailable(USB_RX)) {
		pc_buffer_getMessage(USB_RX, buffer, USART_BUF);
		if (buffer[0] != '\0')
			printf("%s\r\n", buffer);
		printPrompt();
	}
}

int main(void) {

	if (io_init() || periph_init())
		fault();

	printf("Program start\r\n");
	printPrompt();

	while (1) {
		check_input();
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
