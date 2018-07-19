#ifndef __ADCX__H__
#define __ADCX__H__

#include <stdbool.h>
#include <stdint.h>

#define PCF8591_1_ADDR 0x48
#define PCF8591_2_ADDR 0x49

bool adcx_write( uint8_t addr );
bool adcx_start_read(uint8_t addr );
bool adcx_read(uint8_t * val);


#endif
