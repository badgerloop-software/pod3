#include "i2c.h"
#include "honeywell.h"

extern I2C_HandleTypeDef i2c_handle;
int _honeywell_pres_read_val;
int _honeywell_temp_read_val;

bool honeywell_start_read(void){
	return(i2c_start_read(HONEYWELL_I2C_ADDR, 4) == HAL_OK) ? true : false;
}

bool honeywell_read( int *temp, int *pres){
	if(!i2c_read_ready()) return false;

	_honeywell_pres_read_val = i2c_rx[0];
        _honeywell_pres_read_val |= ((i2c_rx[0] & 0x3f) << 8) | i2c_rx[1]; 
        _honeywell_pres_read_val -= 1638; 
        _honeywell_pres_read_val *= 15000; 
        _honeywell_pres_read_val /= 13107;
	*pres = _honeywell_pres_read_val;
	
	_honeywell_temp_read_val |= (i2c_rx[2] << 8) | i2c_rx[3];
       	_honeywell_temp_read_val = _honeywell_temp_read_val >> 5;
       	_honeywell_temp_read_val *=200;
      	_honeywell_temp_read_val /= 2047;
	_honeywell_temp_read_val -= 50;
	*temp = _honeywell_temp_read_val;
	
	i2c_clear_flag(I2C_RX_READY);	

       	return true;
}
