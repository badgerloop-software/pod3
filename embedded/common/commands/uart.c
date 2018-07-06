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
		fprintf(stderr, "This is where you can find another message");
	}
	if (!strcmp("receive", argv[1])) {
		printf("RECEIVING!\n\r");
		uart_receive();
	}
	
	return CMD_SUCCESS;
}


command_status uart_send () {
        int i = 0;
	char message[] =  "I am a message";
	for (i = 0; i < (int)(sizeof(message)/sizeof(message[0])); i++) {
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
