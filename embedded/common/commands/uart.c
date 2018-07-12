#include <stdio.h>
#include "commands.h"
#include "usart.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>


command_status do_uart(int argc, char *argv[]) {
	printf("Doing uart with %d arg\n\r", argc);
	
	if (!strcmp("send", argv[1])) {
		printf("SENDING!\n\r");
		uart_send();
		fprintf(stdout, "This is where you can find another message\n\r");
	}
	if (!strcmp("receive", argv[1])) {
		printf("RECEIVING!\n\r");
		uart_receive();
	}
	
	return CMD_SUCCESS;
}


command_status uart_send (char *message) {
        int i = 0;
	int length = strlen(message);
	for (i = 0; i < length; i++) {
                _putc(USART1, true, message[i]);
	}
	return CMD_SUCCESS;
}


command_status uart_receive () {
	PC_Buffer *rx;
	rx = get_rx(USART1);
	printf("%s\n\r", rx->array);
        char c = ' ';
        while (c != '\n') {
	        _getc(USART1, true, &c);
                printf("%c", c);
                fflush(stdout);
        }
	//	printf("%s\n\r", rx->array);
	return CMD_SUCCESS;
}






COMMAND_ENTRY(
		"uart",
		"uart TODO",
		"TODO.",
		do_uart
	     )
