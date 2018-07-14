#include "pv_data.h"

int pv_DAQ(PV_Data *pvData) {
	MCU_reader(pvData);
	pv_pressure_reader(pvData);
	return 0;
}

int MCU_reader(PV_Data *pvData) {
	//Read MCU info and store
	return 0;
}

int pv_pressure_reader(PV_Data *pvData) {
	//Read ADCs and then collect the data into the struct
	return 0;
}
