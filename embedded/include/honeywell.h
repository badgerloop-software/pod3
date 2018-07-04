#ifndef _HONEYWELL__H__
#define _HONEYWELL__H__

#include <stdbool.h>
#include <stdint.h>

#define HONEYWELL_I2C_ADDR 0x38

bool honeywell_start_read(void);
bool honeywell_read(uint8_t *temp, uint8_t *pres);
bool honeywell_get_staleness(void);



int honeywell_readPressure(void);
int honeywell_readTemperature(void);
bool honeywell_isAlive(void);



#endif
