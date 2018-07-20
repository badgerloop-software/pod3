#include "pv_data.h"
#include <stdio.h>
#include "honeywell.h"
#include "i2c.h"

PV_Data pvData = {MCU_DISABLED, 0, 0};

int pv_DAQ(PV_Data *pvData) {
//	printf("%u", pvData->pvPressure);
	//MCU_reader(pvData);
	harvest_pv_honeywell(pvData);
	return 0;
}


int harvest_pv_honeywell(PV_Data *pvData) {
	int pres, temp;
	if (honeywell_start_read()) {
		if (!i2c_block(I2C_WAITING_RX, ticks)) {
			if (honeywell_read(&temp, &pres)) {
				pvData->temp = temp;
				pvData->pres = pres;
			} else printf("pv honeywell fail\r\n");
		} else printf("failure to block\r\n");
	} else printf("honeywell start read fail\r\n");
	return 0;
}
