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
	
	return SUCCESS;
}


command_status uart_send () {
	char message[] =  "I am a message";
	for (int i = 0; i < (int)(sizeof(message)/sizeof(message[0])); i++) {
		_putc(USART1, true, message[i]);
	}
	return SUCCESS;
}


command_status uart_receive () {
	PC_Buffer *rx;
	rx = get_rx(USART1);
	printf("%s\n\r", rx->array);
	return SUCCESS;
}






COMMAND_ENTRY(
		"uart",
		"uart TODO",
		"TODO.",
		do_uart
	     )
