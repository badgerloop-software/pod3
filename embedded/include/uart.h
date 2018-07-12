#ifndef UART__H
#define UART__H
#include "commands.h"
//#include "system.h"
#include <stm32l4xx_hal.h>
#include <time.h>

command_status uart_send();
command_status uart_receive();

#endif
