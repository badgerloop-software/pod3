#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"
#include "honeywell.h"
#include "i2c.h"
#include <time.h>
#define SEND_BUF_SIZE 128

char packetBuffer[SEND_BUF_SIZE];

int dash_DAQ(Pod_Data_Handle *podData) {
	harvest_honeywell(podData);
	send_data(podData);
	return 0;
}

void set_retro(Pod_Data_Handle *podData, uint8_t retroVal) {
	podData->retro.ui8data = retroVal;
	podData->retro.timestamp = time(NULL);
	podData->retro.freshness = FRESH;
}

void set_lim(Pod_Data_Handle *podData, uint8_t *data) {
    podData->lim1.ui8data = data[3];
    podData->lim2.ui8data = data[4];
    podData->lim3.ui8data = data[5];
	
    podData->lim1.timestamp = time(NULL);
	podData->lim1.freshness = FRESH;

	podData->lim2.timestamp = time(NULL);
	podData->lim2.freshness = FRESH;

	podData->lim3.timestamp = time(NULL);
	podData->lim3.freshness = FRESH;
}

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor;
	harvest_honeywell(pod_data);	
	if (pod_data->current_pressure.freshness == FRESH) {
		pod_data->current_pressure.freshness = NOT_FRESH;
		sensor = &(pod_data->current_pressure);
		char *dataToSend = formatPacket(sensor);
		/*printf("%s", dataToSend);*/
		uart_send(dataToSend);
	}

	if (pod_data->lv_battery_temp.freshness == FRESH) {
		pod_data->lv_battery_temp.freshness = NOT_FRESH;
		sensor = &(pod_data->lv_battery_temp);
		char *dataToSend = formatPacket(sensor);
		//printf("%s\r\n", dataToSend);
		uart_send(dataToSend);
	}

	if (pod_data->retro.freshness == FRESH) {
		pod_data->retro.freshness = NOT_FRESH;
		char *dataToSend = formatPacket(&(pod_data->lv_battery_temp));
		uart_send(dataToSend);
	}
}

void harvest_honeywell(Pod_Data_Handle *pod_data) {
	int pres;
	int temp;
	if (honeywell_start_read()) {
	   	if (!i2c_block(I2C_WAITING_RX, ticks)) {
			if (honeywell_read(&temp, &pres)) {
				pod_data->lv_battery_temp.freshness = FRESH;
				pod_data->lv_battery_temp.timestamp = time(NULL);
				pod_data->lv_battery_temp.i8data    = (int8_t) temp;
				
				pod_data->current_pressure.freshness = FRESH;
				pod_data->current_pressure.timestamp = time(NULL);
				pod_data->current_pressure.ui16data  = (uint16_t) pres;
			} else printf("honeywell read fail\r\n");
		} else printf("failure to block\r\n");
	} else printf("honeywell start read fail\r\n");
}

char *formatPacket(Sensor_Data *sensorData) {
	char *sensName;
	uint8_t u8sensData;
	uint16_t u16sensData;
	int8_t i8sensData;
	int nChars;
	/* grab the values we want to send */
	sensName = sensorData->name;
        /* print the content to the buffer */
	/* the format string will be different by type */
	nChars = -1; 	// To indicate an error
	switch(sensorData->dtype) {
		case DT_UINT8:
			u8sensData = sensorData->ui8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, u8sensData);
			break;
		case DT_UINT16:
			u16sensData = sensorData->ui16data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, u16sensData);
			break;

		case DT_INT8:
			i8sensData = sensorData->i8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%d\n", sensName, i8sensData);
			break;
	}

	if (nChars < 0 || nChars > SEND_BUF_SIZE) {
	        /* TODO this means our print failed or was too big */
	        /* TODO should do something smarter here */
	        printf("Problem encountered while creating sensor data payload\r\n");
	}
	return packetBuffer;
}
