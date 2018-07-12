#include "solenoid.h"
#include "iox.h"
#include "board.h"
#include "uart.h"
#include "usart.h"

#define START_READ_INTERVAL 1000
// format --> 8 bit integer and MSB is Solenoid 7 to 0
void solenoid_handler(Solenoid_States *sol_states) {
	iox_start_read();
	uint8_t stateList;
	if (iox_read(&stateList)) {
		//stateList was actually loaded still need to mask
		sol_states->solenoid_7.state = (stateList & 0x40) >> 6;
		sol_states->solenoid_6.state = (stateList & 0x20) >> 5;
		/*sol_states->solenoid_5.state = (stateList & 0x10) >> 4; NOT WIRED*/
		sol_states->solenoid_4.state = (stateList & 0x08) >> 3;
		/*sol_states->solenoid_3.state = (stateList & 0x04) >> 2; NOT WIRED*/
		sol_states->solenoid_2.state = (stateList & 0x02) >> 1;
		sol_states->solenoid_1.state = (stateList & 0x01);
	};
}
