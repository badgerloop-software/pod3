#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exti.h"
#include "pin_alias.h"
#include "retro.h"
#include "commands.h"
#include "exti.h"
void printStamp (int pin, timeStamp * stamp) {
	int i;
	printf("addr %p\r\n", stamp);
	printf("%-10s:%d\r\n", "Pin", pin);
	printf("%-10s:%lu\r\n", "Previous", stamp->prev);
	printf("%-10s:%lu\r\n", "Current", stamp->curr);
	printf("%-10s:%lu\r\n", "Count", stamp->count);
	for (i = 0; i < AVERAGE_SIZE; i++){
		printf("Filter[%d]:%lu\r\n",i, stamp->filter[i]);
	}	
	
	printf("main\r\n");
	printf("addr %p\r\n", mainRetro);
	printf("%-10s:%lu\r\n", "Previous", mainRetro->prev);
	printf("%-10s:%lu\r\n", "Current", mainRetro->curr);
	printf("%-10s:%lu\r\n", "Count", mainRetro->count);
	
	for (i = 0; i < AVERAGE_SIZE; i++){
		printf("Filter[%d]:%lu\r\n", i,mainRetro->filter[i]);
	}
}

command_status do_exti(int argc, char *argv[]) {
	if (argc < 2) return USAGE;
	uint8_t pin = 0;
	GPIO_TypeDef * port; 

	if(hasGpioAlias(&port, &pin, argv[1])){
		printStamp(pin, getTimeStamps(pin));
		printf("Velocity %d cm/s\r\n", getVelocity());
		return SUCCESS;
	}

	pin = atoi((const char *) argv[1]);
		 	
	printStamp(pin, getTimeStamps(pin));
	printf("Velocity %d cm/s\r\n", getVelocity());

	return SUCCESS;
}
COMMAND_ENTRY("exti", "exti <pin>", "Prints current and previous time stamp of the External Interrupt. Only on interrupt per pin number", do_exti)
