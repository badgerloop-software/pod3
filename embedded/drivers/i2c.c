#include "../include/i2c.h"

//transmit_count keeps track of the current byte being sent over i2c. It is the
//index into the data array for I2C_transmit, and after every byte sent a
//TXIS event generates an interrupt, and the next byte must be written to the 
//I2C_TXDR register (most likely done in the TXIS interrupt handler).  TODO
int transmit_count;

//same thing for receive, except values are read from RXDR in the RXIE handler,
//and written to the receive_data array
int receive_count;

uint8_t *transmit_data;
uint8_t *receive_data;

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
 *I2C_MasterInit
 *
 *Function for complete initialization of I2C. Calls helper functions to
 * to complete task.
 *
*/
bool I2C_MasterInit(uint32_t i2c_base){
	
	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}

	I2C_TypeDef *myi2c = i2c_base;
	
	
	//Software Reset
	myi2c->CR1 &= ~I2C_CR1_PE_Msk;

	//Initialize clock and wait for i2c to be ready by calling I2C_clockConfig
	I2C_clockConfig( i2c_base );
	
	
	//TODO: Other initialization needs to happen, and it'll go here
	//Follow diagram on pg. 1022 of ref. manual

	//Enable I2C
	myi2c->CR1 |= I2C_CR1_PE_Msk;

	//7 bit addresses (rather than 10)
	myi2c->CR2 &= ~I2C_CR2_ADD10_Msk;

	//Enabling TX and RX interrupts
	myi2c->CR1 |= I2C_CR1_TXIE_Msk;
	myi2c->CR1 |= I2C_CR1_RXIE_Msk;

	//Enabling autoend mode, so stop bits are automatically created upon 
	//transmit of nBytes
	myi2c->CR2 |= I2C_CR2_AUTOEND_Msk;
}

bool I2C_SetSlaveAddress(uint32_t i2c_base, uint8_t slave_addr ){
	
	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}

	I2C_TypeDef *myi2c = i2c_base;

	//Slave addr should only be 7 bits long:
	myi2c->CR2 |= (slave_addr & 0x7F);
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

	
	//NEEDS TO BE DONE FOR COMMUNICATION:
	//
	//• Transfer direction: RD_WRN
//• In case of 10-bit address read: HEAD10R bit. HEAD10R must be configure to indicate
//if the complete address sequence must be sent, or only the header in case of a
//direction change.
//• The number of bytes to be transferred: NBYTES[7:0]. If the number of bytes is equal to
//or greater than 255 bytes, NBYTES[7:0] must initially be filled with 0xFF.
}


/*
 *I2C_Transmit
 *
 *	Transmits nBytes to the slave. Should be provided with a data_buffer
 *	that has nBytes entries.
 *
*/
bool I2C_Transmit(uint32_t i2c_base, 
		uint32_t nBytes, uint8_t* data_buffer){
	
	transmit_data = data_buffer;
	transmit_count = 0;

	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}

	I2C_TypeDef *myi2c = i2c_base;

	//Clearing Read/not write bit
	myi2c->CR2 &= ~I2C_CR2_RD_WRN_Msk;

	//Writing the num of bytes to the nbytes field
	myi2c->CR2 |= (nBytes & I2C_CR2_NBYTES_Msk);
		
	//Set data
	myi2c->TXDR |= (data_buffer[ transmit_count ] & I2C_TXDR_TXDATA_Msk );

	//Set start bit, hardware automatically generates start bit, sends slave 
	//addr and R/W bit
	myi2c->CR2 |= I2C_CR2_START_Msk;

	//Stop bit is automatically generated due to AUTOEND being enabled
	

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

	receive_data = data_buffer;
	receive_count = 0;

	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}

	I2C_TypeDef *myi2c = i2c_base;

	//Setting Read/not write bit
	myi2c->CR2 |= I2C_CR2_RD_WRN_Msk;

	//Writing the num of bytes to the nbytes field
	myi2c->CR2 |= (nBytes & I2C_CR2_NBYTES_Msk);
		
	//Set start bit, hardware automatically generates start bit, sends slave 
	//addr and R/W bit
	myi2c->CR2 |= I2C_CR2_START_Msk;

	//Stop bit is automatically generated due to AUTOEND being enabled
	
	//Data must be read out from RXDR register whenever an RXIE interrupt is 
	//received, and stored in the receive_data array
}	


/*
 *I2C_clockConfig //TODO: going to need help with this one
 *	
 *	Initializes the clock
 *
*/
bool I2C_clockConfig(uint32_t i2c_base, bool state ){

}

/*
 *I2C_disable_NVIC
 *	
 *	Disable interrupts for Atomic operation.
 *
*/
bool I2C_disable_NVIC(uint32_t i2c_base){

	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}


}


/*
 *I2C_enable_NVIC
 *	
 *	Enable interrupts to end Atomic operation.
 *
*/
bool I2C_enable_NVIC(uint32_t i2c_base){

	if( !(I2C_verifyBase(i2c_base) )){
		return false;
	}
}



