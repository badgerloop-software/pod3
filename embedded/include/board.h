#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>

typedef enum _board_role {
	DEV = 0,
	DASH = 1,
	NAV = 2,
	PV = 3
} BOARD_ROLE;

int io_init(void);
int periph_init(BOARD_ROLE role);
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
