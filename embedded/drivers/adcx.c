#include <stdio.h>
#include <string.h>
#include <adcx.h>
#include "pcf8591.h"
#include "i2c.h"

uint8_t _i2cadc_read_pos;
bool adcx_read_stale = true;

bool adcx_write( uint8_t addr, uint8_t nBytes, uint8_t* data ){

    if( i2c_start_write( addr, nBytes, data) == HAL_OK) {
        adcx_read_stale = true;
        return true;
    }
    printf("ADCx Write Failure");
    return false;
}

//use number of data bytes you want, the first byte is unused
bool adcx_start_read(uint8_t addr, uint8_t nBytes ){
    
    return (i2c_start_read(addr, (nBytes + 1)) == HAL_OK) ? true : false;
}

//use number of data bytes you want, the first byte is unused
bool adcx_read(uint8_t * val, uint8_t nBytes) {
    while (!i2c_read_ready() ){}
    int i;
    
    for( i = 0; i < nBytes; i++){
    	val[i] = i2c_rx[i+1];
    }

    i2c_clear_flag(I2C_RX_READY);
    adcx_read_stale = false;
    return true;
}

