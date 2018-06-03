#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>
#include "system.h"
#include "stm32l4xx_hal.h"

int io_init(void);
int periph_init();
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
