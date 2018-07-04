#include "i2c.h"
#include "honeywell.h"

extern I2C_HandleTypeDef i2c_handle;
uint8_t _honeywell_pres_read_val;
uint8_t _honeywell_temp_read_val;
bool honeywell_read_stale = true;

bool honeywell_start_read(void){
	return(i2c_start_read(HONEYWELL_I2C_ADDR, 4) == HAL_OK) ? true : false;
}

bool honeywell_get_staleness(void) {
	return honeywell_read_stale;
}



bool honeywell_read(uint8_t *temp, uint8_t *pres){
	if(!i2c_read_ready()) return false;
	printf("TEMP INITIAL %d", *temp);
	_honeywell_pres_read_val = i2c_rx[0];
	
        _honeywell_pres_read_val |= ((i2c_rx[0] & 0x3f) << 8) | i2c_rx[1]; 
        _honeywell_pres_read_val -= 1638; 
        _honeywell_pres_read_val *= 15000; 
	_honeywell_pres_read_val /= 13107;
	* pres = _honeywell_pres_read_val;
	
	_honeywell_temp_read_val |= (i2c_rx[2] << 8) | i2c_rx[3];
       	_honeywell_temp_read_val = _honeywell_temp_read_val >> 5;
       	_honeywell_temp_read_val *=200;
      	_honeywell_temp_read_val /= 2047;
	_honeywell_temp_read_val -= 50;
	*temp = _honeywell_pres_read_val;
	
	i2c_clear_flag(I2C_RX_READY);	
	honeywell_read_stale = false;

       	return true;
}

int honeywell_readTemperature(void){

       uint32_t ticks_start = ticks;

       int retval = 0;
       
       if(i2c_start_read(HONEYWELL_I2C_ADDR, 4) != HAL_OK) return -1;
       
       if((i2c_block(I2C_WAITING_RX, ticks_start)) || i2c_errors_present()) return -1;

	if(!i2c_read_ready()) return false;
       
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
