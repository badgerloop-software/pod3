#ifndef _BOARD__H__
#define _BOARD__H__

#include <stdbool.h>

typedef enum _board_role {
	DEV = 0,
	DASH = 1,
	NAV = 2,
	PV = 3
} BOARD_ROLE;

typedef enum _receiving_boards {
	DEV_REC = 0,
	DASH_REC = 1,
	NAV_REC = 2,
	PV_REC = 3,
	CCP_NAV_REC = 4,
	CCP_PV_REC = 5,
	NAV_PV_REC = 6,
	ALL = 7
} RECEIVING_BOARD;

typedef enum _can_message_type {
	TEST_MESSAGE = 0,
	
} CAN_MESSAGE_TYPE;


int io_init(void);
int periph_init(BOARD_ROLE role);
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
