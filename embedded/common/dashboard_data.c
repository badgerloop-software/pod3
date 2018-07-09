#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"
#include "honeywell.h"
#include "i2c.h"
#define SEND_BUF_SIZE 128

char packetBuffer[SEND_BUF_SIZE];

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor;
	int pres;
	int temp;
	if (!honeywell_start_read()) printf("honeywell start read fail\r\n");
	if (i2c_block(I2C_WAITING_RX, ticks)) printf("failure to block\r\n");
	if (!honeywell_read(&temp, &pres)) printf("honeywell read fail\r\n");
	pod_data->lv_battery_temp.ui8data = (int8_t) temp;
	pod_data->current_pressure.ui16data = (uint16_t) pres;
	printf("PRESSURE: %u : TEMP: %u\r\n", pres, temp);
	
	if (pod_data->current_pressure.freshness == NOT_FRESH) {
		//pod_data->pressure.freshness = NOT_FRESH;
		sensor = &(pod_data->current_pressure);
		char *dataToSend = formatPacket(sensor);
		//printf("%s", dataToSend);
		uart_send(dataToSend);	
	}

	if (pod_data->lv_battery_temp.freshness == FRESH) {
		//pod_data->temperature.freshness = NOT_FRESH;
		sensor = &(pod_data->lv_battery_temp);
		char *dataToSend = formatPacket(sensor);
		//printf("%s\r\n", dataToSend);
		uart_send(dataToSend);
	}
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