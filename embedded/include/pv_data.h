#ifndef PV_DATA__H
#define PV_DATA__H
#include "nav_data.h"
#include <stdint.h>

#define MCU_ENABLED  1
#define MCU_DISABLED 0 

typedef struct {
	uint8_t MCUStatus; /* Should only be set with above defines*/
	uint16_t pres;
	uint16_t temp;
} PV_Data;

extern state_box pv_stateVal;
int MCU_reader(PV_Data *);
int pv_DAQ(PV_Data *);
int harvest_pv_honeywell(PV_Data *);
#endif
