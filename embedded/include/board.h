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
	CAN_TEST_MESSAGE,
	LV_HEARTBEAT,

	CCP_FAULT,
	CCP_WARNING,
    CCP_SOLENOID_COMMAND,
    CCP_MCU_ENABLE,	
	CCP_RMS_POWER,
	
	PV_WARNING,
	PV_SHUTDOWN_CIRCUIT_STATUS,	
	PV_PRESSURE,

	NAV_FAULT,			
	NAV_WARNING,
	NAV_TAPE,		
	NAV_SHOULD_STOP,
	NAV_PRES_1,
	NAV_PRES_2,
	NAV_PRES_3,
    NAV_ADC,
	NAV_SOLENOID_1,
	NAV_ACCEL_VEL_POS,
	CURR_STATE
	
} CAN_MESSAGE_TYPE;

typedef enum _bms_telemetry {
	BMS_PACK_STATE_MESSAGE = 0x6b0,
	BMS_PACK_TEMP_MESSAGE = 0x6b1,
	BMS_RELAY_STATE_MESSAGE = 0x653,
	BMS_PACK_CCL = 0x652,
	BMS_CELL_VOLT_MESSAGE = 0x651,
	BMS_SOC_MESSAGE = 0x650,
//	BMS_PACK_CURRENT_MESSAGE = //TODO extended IDs? 
//	BMS_PACK_VOLT_MESSAGE = 
} BMS_MESSAGE_TYPE;

extern const int board_type;

int io_init(void);
int periph_init(BOARD_ROLE role);
void fault(void);
void blink_handler(unsigned int blink_int);
void set_led(bool state);

void post(const char *app_name);

#endif
