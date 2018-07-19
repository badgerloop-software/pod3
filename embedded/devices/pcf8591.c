#include <stdio.h>
#include <string.h>
#include <adcx.h>
#include "pcf8591.h"
#include "i2c.h"

//IDEA:
//
// Before reading certain channel, write new control byte to the channel, then read from it

uint16_t _i2cadc_read_val;
uint8_t _i2cadc_read_pos;
bool adcx_read_stale = true;
uint8_t adcx_channel = 0;

bool adcx_write( uint8_t addr, uint8_t channel ){
    adcx_channel = channel;
    if( i2c_start_write( addr, 1, &adcx_channel) == HAL_OK) {
        adcx_read_stale = true;
        return true;
    }
    printf("ADCx Write Failure");
    return false;
}

bool adcx_start_read(uint8_t addr, uint8_t* channel_num ){
    
    *channel_num = adcx_channel;

    HAL_StatusTypeDef status = i2c_start_read(addr,1);
    printf( "ADCx Read Status: %d\r\n", status );
    return false;
    //return (i2c_start_read(addr, 1) == HAL_OK) ? true : false;
}

bool adcx_read(uint16_t * val) {
    if (!i2c_read_ready() ){
        printf("ADCx Read Ready\r\n");
        return false;
    }
    _i2cadc_read_val = i2c_rx[0];
    i2c_clear_flag(I2C_RX_READY);
    *val = _i2cadc_read_val;
    adcx_read_stale = false;
    return true;
}

/*
bool adcx_read(uint8_t addr, uint8_t * val) {
		
	_i2cadc_read_pos = *val;
	_i2cadc_read_val = 0;
	uint32_t ticks_start = ticks;

	printf("in function\r\n");

	if (i2c_start_write(addr, 1, &_i2cadc_read_pos) == HAL_OK){
		if (!i2c_read_ready()) return false;
        if( i2c_start_read(addr, 2) != HAL_OK ) return false; 
		if (i2c_block(I2C_WAITING_RX, ticks_start) || i2c_errors_present()) return false;
		_i2cadc_read_val = i2c_rx[0];
		printf("i2c read value %x\r\n", _i2cadc_read_val);
		*val = i2c_rx[1];
	}
	return true;
}
*/
