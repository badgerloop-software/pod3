#include <stdio.h>
#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "can.h"
#include "pv_data.h"
#include "nav_data.h"
#include "state_machine.h"
#include "state_handlers.h"

#define BLINK_INTERVAL	250
#define CTRL_INTERVAL   100

const int board_type = PV;
uint32_t nav_timestamp = 0;
uint32_t dash_timestamp = 0;
uint32_t pv_timestamp = 0;

PV_Data pvData = {MCU_DISABLED, 0, 0};
state_box pv_stateVal = {3, 0};
/* Nucleo 32 I/O */

//Shutdown Circuit
FILL_GPIO(MSTR_SW_FDBK, GPIOB, 0, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(E_STOP_FDBK, 	GPIOB, 7, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(PV_LIM_FDBK, 	GPIOB, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)

FILL_GPIO(HVD_FDBK, 	GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(BMS_STAT_FDBK, GPIOC, 14, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(IMD_STAT_FDBK, GPIOC, 15, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(HV_EN_FDBK, 	GPIOA, 8, INPUT, LOW_SPEED, NONE, true, OTHER)
//HVD_MCU is a spare limit switch input
FILL_GPIO(HVD_MCU,  GPIOA, 4, INPUT, LOW_SPEED, NONE, true, OTHER)

//SOLENOIDS
FILL_GPIO(SOL1, 	GPIOA, 5, OUTPUT, LOW_SPEED, NONE, true, VALVE)
FILL_GPIO(SOL2, 	GPIOA, 1, OUTPUT, LOW_SPEED, NONE, true, VALVE)

//Voltage Divider
FILL_GPIO(VD2, 	GPIOA, 3, INPUT, LOW_SPEED, NONE, true, OTHER)

//MCU
//FILL_GPIO(LIM_MCU, 		GPIOA, 2, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(MCU_HV_EN, 	GPIOA, 6, OUTPUT, LOW_SPEED, NONE, true, OTHER)

//MISC
//High Voltage Disconnect Status
FILL_GPIO(HVD_STATUS, 	GPIOA, 7, INPUT, LOW_SPEED, NONE, true, OTHER)
//Motor GPIO circuit output
FILL_GPIO(DIN8, 		GPIOB, 5, OUTPUT, LOW_SPEED, NONE, true, OTHER)

FILL_GPIO(HVD_FDBK, 	GPIOB, 4, OUTPUT, LOW_SPEED, NONE, true, OTHER)

inline void printPrompt(void) {
	fputs("[pv-build] $ ", stdout);
	fflush(stdout);
}

int pv_init(void) {

	/* pv specific initializations */

	return 0;
}

int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(PV) || pv_init())
		fault();

	rx = get_rx(USB_UART);

	post("Pressure Vessel");

	printPrompt();
	unsigned int lastDAQ = 0, lastState = 0, lastTelem = 0, lastHrtbt = 0;
	unsigned int currDAQ = 0, currState = 0, currTelem = 0, currHrtbt = 0;
	while (1) {

		currDAQ = (ticks + 10) / 100;
		currState = (ticks + 20) / 100;
		currTelem = (ticks + 30) / 100;
		currHrtbt = (ticks + 40) / 100;

        if( ticks - dash_timestamp >= 500) {
        
            if( state_handle.curr_state <= READY ){
                change_state( PRE_RUN_FAULT );
            }else if( state_handle.curr_state > READY && state_handle.curr_state < POST_RUN ){
                change_state( RUN_FAULT );
            }else{
                change_state( POST_RUN_FAULT );
            }
            //TODO Send out right away
            state_machine_handler();

        }

		if( can_read() == HAL_OK) board_can_message_parse( BADGER_CAN_ID, RxData);
		if( currDAQ != lastDAQ ){
			lastDAQ = currDAQ;
		}
		if( currState != lastState ){
			state_machine_handler();
			lastState = currState;
		}
		if( currTelem != lastTelem ){
			lastTelem = currTelem;
		}
		if( currHrtbt != lastHrtbt ){
			lastHrtbt = currHrtbt;
		}
		
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
