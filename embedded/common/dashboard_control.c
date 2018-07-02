#include <stdio.h>
#include "dashboard_control.h"
#include "usart.h"

void process_control(char *buf) {
	/* TODO determine the command and execute*/
	puts(buf);
}

char buffer[USART_BUF];
void check_incoming_controls(PC_Buffer *rx) {

	/* only something to do if a message is waiting */
	if (pc_buffer_messageAvailable(rx)) {

		pc_buffer_getMessage(rx, buffer, USART_BUF);

		/* process the command string if it's not empty */
		if (buffer[0] != '\0') process_control(buffer);

	}
}
