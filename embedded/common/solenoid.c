#include "solenoid.h"
#include "iox.h"
#include "board.h"
#include "uart.h"
#include "usart.h"
#include "nav_data.h"
#include "i2c.h"
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

bool init_solenoids() {
    uint8_t data = 0xD1;
    uint32_t ticks_start = ticks;
    printf("Init Solenoids\r\n");
    if( i2c_start_write( 0x20, 1, &data) != HAL_OK ){
	return false;
    }
    if( i2c_block(I2C_WAITING_TX, ticks_start) || i2c_errors_present() ){
	return false;
    }
    return true;
}

bool actuate_brakes() {
    uint8_t data = 0xC0;
    uint32_t ticks_start = ticks;
    printf("Primary Solenoids\r\n");
    if( i2c_start_write( 0x20, 1, &data) != HAL_OK ){
	return false;
    }
    if( i2c_block(I2C_WAITING_TX, ticks_start) || i2c_errors_present() ){
	return false;
    }
    return true;
}
/*
void unactuate_brakes() {
    uint8_t data = 0xEE;
    i2c_start_write( 0x20, 1, &data);  
    return;
}
*/

bool actuate_sec_brakes() {
    uint8_t data = 0x88;
    printf("Sec Solenoids\r\n");
    uint32_t ticks_start = ticks;
    if( i2c_start_write( 0x20, 1, &data) != HAL_OK ){
	return false;
    }
    if( i2c_block(I2C_WAITING_TX, ticks_start) || i2c_errors_present() ){
	return false;
    }
    return true;
}

bool vent_brakes() {
    uint8_t data = 0x08;
    uint32_t ticks_start = ticks;
    printf("Vent Solenoids\r\n");
    if( i2c_start_write( 0x20, 1, &data) != HAL_OK ){
	return false;
    }
    if( i2c_block(I2C_WAITING_TX, ticks_start) || i2c_errors_present() ){
	return false;
    }
    return true;
}
