#include "solenoid.h"
#include "iox.h"
#include "board.h"
#include "uart.c"
#include "usart.c"

#define START_READ_INTERVAL 1000
// format --> 8 bit integer and MSB is Solenoid 7 to 0
solenoid_handler(Solenoid_States *sol_states) {
	iox_start_read();
	uint8_t stateList;
	if (iox_read(&stateList)) {
		//stateList was actually loaded still need to mask
		sol_states->solenoid_8.state = stateList >> 64;
		sol_states->solenoid_7.state = stateList >> 32;
		sol_states->solenoid_6.state = stateList >> 16;
		sol_states->solenoid_5.state = stateList >> 8;
		sol_states->solenoid_4.state = stateList >> 4;
		sol_states->solenoid_3.state = stateList >> 2;
		sol_states->solenoid_2.state = stateList >> 1;
		sol_states->solenoid_1.state = stateList
	};
}
