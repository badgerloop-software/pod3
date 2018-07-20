#include "solenoid.h"
#include "iox.h"
#include "board.h"
#include "uart.h"
#include "usart.h"
#include "nav_data.h"

#include <stdio.h>
 
#define START_READ_INTERVAL 1000
// format --> 8 bit integer and MSB is Solenoid 7 to 0
void harvest_solenoids(Solenoid_States *sol_states) {

	uint8_t stateList;
	
    if (iox_read(&stateList)) {
		sol_states->solenoid_7.state = (stateList & 0x40) >> 6;
    	sol_states->solenoid_6.state = (stateList & 0x20) >> 5;
	 	/*sol_states->solenoid_5.state = (stateList & 0x10) >> 4; NOT WIRED*/
		sol_states->solenoid_4.state = (stateList & 0x08) >> 3;
		/*sol_states->solenoid_3.state = (stateList & 0x04) >> 2; NOT WIRED*/
		sol_states->solenoid_2.state = (stateList & 0x02) >> 1;
		sol_states->solenoid_1.state = (stateList & 0x01);
	}
}

int change_solenoid(int name, int state) {
	if (state == ACTUATED) {
		iox_set(name);	
	} else if (state == NOT_ACTUATED) {
		iox_clear(name);	
	}
	return 0;
}


void actuate_brakes() {
	iox_set(PRIM_BRAKING_1);
	iox_set(PRIM_BRAKING_2);	
}

void actuate_sec_brakes() {
	iox_set(SEC_BRAKING_1);
	iox_set(SEC_BRAKING_2);
}
