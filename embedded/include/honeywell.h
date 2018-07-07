#ifndef _HONEYWELL__H__
#define _HONEYWELL__H__

#include <stdbool.h>
#include <stdint.h>

#define HONEYWELL_I2C_ADDR 0x38

bool honeywell_start_read(void);
bool honeywell_read(int *temp, int *pres);

#endif
