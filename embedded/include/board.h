#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>

extern uint8_t board_num;

typedef enum board_type {
	DEV = 0,
	DASH = 1,
	NAV = 2,
	PV = 3
} BOARD_TYPE;

int io_init(void);
int periph_init(void);
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
