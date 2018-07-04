#include "i2c.h"
#include "honeywell.h"

extern I2C_HandleTypeDef i2c_handle;


bool honeywell_isAlive(void){
       return (HAL_I2C_IsDeviceReady(&i2c_handle, HONEYWELL_I2C_ADDR << 1, 1, 500) == HAL_OK) ? true: false;
}
int honeywell_readTemperature(void){

       uint32_t ticks_start = ticks;

       int retval = 0;
       
       if(i2c_start_read(HONEYWELL_I2C_ADDR, 4) != HAL_OK) return -1;
       
       if((i2c_block(I2C_WAITING_RX, ticks_start)) || i2c_errors_present()) return -1;

               
       
       retval |= (i2c_rx[2] << 8) | i2c_rx[3];
       retval = retval >> 5;
       retval *=200;
       retval /= 2047;
       return retval -50;      
}

int honeywell_readPressure(void){
       
       uint32_t ticks_start = ticks;

       int retval = 0;
       if(i2c_start_read(HONEYWELL_I2C_ADDR, 4) != HAL_OK) return -1;
       
       if((i2c_block(I2C_WAITING_RX, ticks_start)) || i2c_errors_present()) return -1;

         
        retval = i2c_rx[0]; 
        retval |= ((i2c_rx[0] & 0x3f) << 8) | i2c_rx[1]; 
        retval -= 1638; 
        retval *= 15000; 
	return retval / 13107;
}
