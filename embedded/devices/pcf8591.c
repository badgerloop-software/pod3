#include <stdio.h>
#include <string.h>
#include <adcx.h>
#include "pcf8591.h"
#include "i2c.h"
#include <stdbool.h>

uint8_t pSense[8];

bool i2adc_read( uint8_t addr ){
    int i;
    for( i = 0; i < 8; i++){
        pSense[i] = 0;
    }

    if( addr != 0x48 && addr != 0x49){
        printf("Wrong Address passed in.\r\n");
        return false;
    }

    if(!adcx_start_read(addr, 4) ){
        printf("%s: could not read from i2c:\r\n",__func__);
        i2c_dump();
        return false;
    }
    if (i2c_block(I2C_WAITING_RX, ticks)){
        printf("%s waiting for read timed out", __func__);
        i2c_dump();
        return false;
    }

    if( addr == 0x48 ){
        if (!adcx_read( pSense, 4 )){
            printf("%s call to adcx_read failed\r\n", __func__);
            return false;
        }
        for( i = 0; i < 4; i++){
            printf("ADCx value read 0x%x from Channel #%d\r\n", pSense[i], i);
        }
    }
    else{
        if (!adcx_read( &(pSense[4]), 4 )){
            printf("%s call to adcx_read failed\r\n", __func__);
            return false;
        }

        for( i = 4; i < 8; i++){
            printf("ADCx value read 0x%x from Channel #%d\r\n", pSense[i], i);
        }

    }
    return true;
}

bool i2adc_write(uint8_t addr){
        
    uint8_t control_byte = 4;

    if( addr != 0x48 && addr != 0x49){
        printf("Wrong Address passed in.\r\n");
        return false;
    }
    
    if(!adcx_write( addr, 1, &control_byte ) ){
        printf("ADCx Write Error\r\n");
        return false;
    }
    return true;
}


