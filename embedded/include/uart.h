#ifndef UART__H
#define UART__H
#include "commands.h"
#include "system.h"
#include <stm32l4xx_hal.h>
#include <time.h>

#define FRESH 		0
#define NOT_FRESH 	1

command_status uart_send();
command_status uart_receive();

typedef struct {
	char *name;
	uint8_t data;
	time_t timestamp;
	uint8_t freshness;
} Sensor_Data;


typedef struct {
	Sensor_Data pressure;
	Sensor_Data temperature;	
} Pod_Data_Handle;


#endif
