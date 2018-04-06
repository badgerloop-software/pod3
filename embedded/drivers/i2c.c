#include "../include/i2c.h"

bool I2C_verifyBase(uint32_t i2c_base)
{
	switch(i2c_base)
    {
    	case I2C1_BASE: return true;

        case I2C2_BASE: return true;
               
        case I2C3_BASE: return true;        

		case I2C4_BASE: return true;

      	default: return false;         
    }
} 

/*
 *I2C_Master_Receive_Byte
 *	
 *	Reads one byte from the slave.
 *
*/
bool I2C_Master_Receive_Byte(I2C_TypeDef* i2c){

}


/*
 *I2C_Master_Transmit_Byte
 *	
 *	Transmits one byte to the slave.
 *
*/
bool I2C_Master_Transmit_Byte(I2C_TypeDef* i2c){

}


/*
 *I2C_transmit_receive_config
 *
 *	Should call I2C_Master_Transmit_Byte for each byte of data (nBytes times).
 *	Each byte of data is stored as an entry in the data_buffer.
 *
*/
bool I2C_Transmit(uint32_t i2c_base, 
		uint32_t nBytes, uint8_t* data_buffer){

}	


/*
 *I2C_Receive
 *
 *	Should call I2C_Master_Receive_Byte for each byte of data (nBytes times).
 *	Each byte of data is stored as an entry in the data_buffer.
 *
*/
bool I2C_Receive(uint32_t i2c_base, 
		uint32_t nBytes, uint8_t* data_buffer){

}	


/*
 *I2C_clockConfig
 *	
 *	Initializes the clock
 *
*/
bool I2C_clockConfig(uint32_t i2c_base, bool state ){

}


/*
 *I2C_enable
 *	
 *	Enable the I2C for operation.
 *
*/
bool I2C_enable(bool enable, uint32_t i2c_base){

}




/*
 *I2C_disable_NVIC
 *	
 *	Disable interrupts for Atomic operation.
 *
*/
bool I2C_disable_NVIC(uint32_t i2c_base){

}


/*
 *I2C_enable_NVIC
 *	
 *	Enable interrupts to end Atomic operation.
 *
*/
bool I2C_enable_NVIC(uint32_t i2c_base){

}



