#include <stdio.h>
#include <string.h>
#include <adcx.h>
#include "pcf8591.h"
#include "i2c.h"
#include <stdbool.h>
#include "nav_data.h"

extern Nav_Data navData;

//Address 0x48
//AIN3 double pTank;
//AIN2 double  pLine; 
//AIN1 double pAct;

//Address 0x49
//AIN2: double sTank;
//AIN1: double sLine; 
//AIN0: double sAct;

uint8_t pSense[8];
double pVolt[8];

bool i2adc_read( uint8_t addr ){
    int j;

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
            for( j = 0; j < 4; j++){
                pVolt[j] = pSense[j] * 12.89 /1000;
                //printf("ADCx value read %f Volts from Channel #%d\r\n", pVolt[j], j);
            }
        }
        else{
            if (!adcx_read( &(pSense[4]), 4 )){
                printf("%s call to adcx_read failed\r\n", __func__);
                return false;
            }
            for( j = 4; j < 8; j++){
                pVolt[j] = pSense[j] * 12.89 /1000;

                printf("ADCx value read %f Volts from Channel #%d\r\n", pVolt[j], j);
    	    }
        }
	
    navData.linePressures.pres_1 = pSense[7] * 1.1718; //sAct
    navData.linePressures.pres_2 = pSense[4] * 1.1718; //sLine
    navData.linePressures.pres_3 = pSense[5] * 7.8125; //sTank
    navData.linePressures.pres_4 = pSense[1] * 1.1718; //pAct
    navData.linePressures.pres_5 = pSense[2] * 1.1718; //pLine
    navData.linePressures.pres_6 = pSense[3] * 7.8125; //pTank
            
    //printf("ADCx value read %f PSI from Sec. Actuator\r\n", navData.linePressures.pres_1 );
    //printf("ADCx value read %f PSI from Sec. Line\r\n", navData.linePressures.pres_2 );
    //printf("ADCx value read %f PSI from Sec Tank\r\n", navData.linePressures.pres_3 );
    //printf("ADCx value read %f PSI from Prim. Actuator \r\n", navData.linePressures.pres_4 );
    //printf("ADCx value read %f PSI from Prim. Line \r\n", navData.linePressures.pres_5 );
    //printf("ADCx value read %f PSI from Prim. Tank \r\n", navData.linePressures.pres_6 );
    
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


