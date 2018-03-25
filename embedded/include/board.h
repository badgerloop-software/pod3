#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>

int io_init(void);
int periph_init(void);
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
