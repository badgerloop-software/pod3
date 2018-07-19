#include <stdio.h>
#include <string.h>
#include <adcx.h>
#include "pcf8591.h"
#include "i2c.h"
#include <stdbool.h>

//Address 0x48
double pTank; //AIN3
double pLine; //AIN2
double pAct; //AIN1

//Address 0x49
double sTank; //AIN2
double sLine; //AIN1
double sAct; //AIN0

uint8_t pSense[8];
double pVolt[8];

bool i2adc_read( uint8_t addr ){
    int i;
    int j;
        for( i = 0; i < 8; i++){
            pSense[i] = 0;
        }

        if( addr != 0x48 && addr != 0x49){
            printf("Wrong Address passed in.\r\n");
            return false;
        }

    for( j = 0; j < 4; j++ ){
        if(!adcx_start_read(addr, 1) ){
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
            if (!adcx_read( pSense, 1 )){
                printf("%s call to adcx_read failed\r\n", __func__);
                return false;
            }
            pVolt[j] = pSense[j] * 12.89 /1000;
            printf("ADCx value read %f Volts from Channel #%d\r\n", pVolt[j], j);
        }
        else{
            if (!adcx_read( &(pSense[j+4]), 1 )){
                printf("%s call to adcx_read failed\r\n", __func__);
                return false;
            }
            pVolt[j+4] = pSense[j+4] * 12.89 /1000;
            printf("ADCx value read %f Volts from Channel #%d\r\n", pVolt[j+4], j+4);
    	}
    }
	
    sAct = pVolt[7];
    sLine = pVolt[4];
    sTank = pVolt[5];
    pAct = pVolt[1];
    pLine = pVolt[2];
    pTank = pVolt[3];
            
    printf("ADCx value read %f Volts from Sec. Actuator\r\n", sAct);
    printf("ADCx value read %f Volts from Sec. Line\r\n", sLine);
    printf("ADCx value read %f Volts from Sec Tank\r\n", sTank);
    printf("ADCx value read %f Volts from Prim. Actuator \r\n", pAct);
    printf("ADCx value read %f Volts from Prim. Line \r\n", pLine);
    printf("ADCx value read %f Volts from Prim. Tank \r\n", pTank);
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


