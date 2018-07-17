#include <stdio.h>
#include <time.h>
#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "uart.h"
#include "dashboard_data.h"
#include "dashboard_control.h"
#include "can.h"

#define BLINK_INTERVAL	250
#define CTRL_INTERVAL   100

const int board_type = DASH;

Pod_Data_Handle podData = {
	 .current_pressure = {"current_pressure", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
	 .lv_battery_temp =  {"lv_battery_temp", 0,0, 0, 0, NOT_FRESH, DT_INT8},
	 .position = {"position", 0, 0, 0, 0, NOT_FRESH, DT_INT8},
	 .velocity = {"velocity", 0, 0, 0, 0, NOT_FRESH, DT_INT8},
	 .acceleration = {"acceleration", 0, 0, 0, 0, NOT_FRESH, DT_INT8},
	 .tube_pressure = {"tube_pressure", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
	 .retro = {"retro", 0, 0, 0, 0, NOT_FRESH, DT_UINT8},
	 .solenoids = {"solenoids", 0, 0, 0, 0, NOT_FRESH, DT_UINT8},
	 .linePressures = {{"line_pres_1", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},	  								   {"line_pres_2", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
		               {"line_pres_3", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
					   {"line_pres_4", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},	  								   {"line_pres_5", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
		               {"line_pres_6", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},
					   {"line_pres_7", 0, 0, 0, 0, NOT_FRESH, DT_UINT16},	  								   {"line_pres_8", 0, 0, 0, 0, NOT_FRESH, DT_UINT16}}
};

/* Nucleo 32 I/O */
//THERM1: between batteries 1 and 2
//THERM2: between batteries 2 and 3
FILL_GPIO(THERM1,     GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(THERM2,	  GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR)

//SENSOR
//VOLT_SENSE is 12V battery voltage
//CURR_SENSE is 12V battery current draw, charging not included
FILL_GPIO(VOLT_SENSE, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(CURR_SENSE, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR)

//UART
FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER )

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )

//SD
FILL_GPIO(SD_CS,	 GPIOB, 0, OUTPUT, LOW_SPEED, NONE, true, OTHER )

inline void printPrompt(void) {
	fputs("[dash-build] $ ", stdout);
	fflush(stdout);
}

int dash_init() {
	

	return 0;
}

int main(void) {

	PC_Buffer *rx;
	PC_Buffer *ctrl_rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(DASH) || dash_init())
		fault();

	rx = get_rx(USB_UART);
	ctrl_rx = get_rx(USART1);

	post("Dashboard");
	printPrompt();
	unsigned int lastDAQ = 0, lastState = 0, lastTelem = 0, lastHrtbt = 0;
	while (1) {
		if (can_read() == HAL_OK) ccp_parse_can_message(BADGER_CAN_ID, RxData, &podData);
		if (((ticks + 10) % CTRL_INTERVAL == 0) && lastDAQ != ticks) {
			lastDAQ = ticks;
			if (dash_DAQ(&podData)) printf("DAQ Failure");
		}	
		if (((ticks + 15) % CTRL_INTERVAL == 0) && lastState != ticks) {
			lastState = ticks;
			//state_machine_logic();
			//check if new state is needed
		}
		if (((ticks + 20) % CTRL_INTERVAL == 0) && lastTelem != ticks ) {
			lastTelem = ticks;
			send_data(&podData);
			//board_telemetry_send(board_type);
			//CCP sends telem to Pi
		}
		if (((ticks + 25) % CTRL_INTERVAL == 0) && lastHrtbt != ticks) {
			lastHrtbt = ticks;
			//board_telemetry_send(board_type); <-- maybe a diff func for heartbeat?
			//Nav sends heartbeat
		}
		check_input(rx);
		check_incoming_controls(ctrl_rx);
		blink_handler(BLINK_INTERVAL);
	}


	return 0;
}
