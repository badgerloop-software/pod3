#ifndef _HONEYWELL__H__
#define _HONEYWELL__H__

#include <stdbool.h>
#include <stdint.h>

#define HONEYWELL_I2C_ADDR 0x38

bool honeywell_isAlive(void);
int honeywell_readPressure(void);
int honeywell_readTemperature(void);




#endif
