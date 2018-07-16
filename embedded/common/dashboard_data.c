#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"
#include "honeywell.h"
#include "i2c.h"
#include <time.h>
#include "can.h"

#define SEND_BUF_SIZE 128

char packetBuffer[SEND_BUF_SIZE];

int dash_DAQ(Pod_Data_Handle *podData) {
//	ccp_parse_can_message(BADGER_CAN_ID, RxData, podData);	
	harvest_honeywell(podData);
	return 0;
}

void set_retro(Pod_Data_Handle *podData, uint8_t retroVal) {
	//printf("RETRO: %u\r\n", retroVal);
	podData->retro.ui8data = retroVal;
	podData->retro.timestamp = time(NULL);
	podData->retro.freshness = FRESH;
}

void set_solenoid_value(Pod_Data_Handle *podData, uint8_t solenoidsVal) {
	podData->solenoids.ui8data = solenoidsVal;
	podData->solenoids.timestamp = time(NULL);
	podData->solenoids.freshness = FRESH;
}

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor;
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
		char *dataToSend = formatPacket(&(pod_data->retro));
		uart_send(dataToSend);
	}

	if (pod_data->solenoids.freshness == FRESH) {
		pod_data->solenoids.freshness = NOT_FRESH;
		Sensor_Data sol1 = {"solenoid_1", 0, (pod_data->solenoids.ui8data & 0x1), 0, 0, 0, DT_UINT8};
		char *sol1str = formatPacket(&sol1);
		Sensor_Data sol2 = {"solenoid_2", 0, (pod_data->solenoids.ui8data & 0x2) >> 1, 0, 0, 0, DT_UINT8};
		char *sol2str = formatPacket(&sol2);
		Sensor_Data sol4 = {"solenoid_4", 0, (pod_data->solenoids.ui8data & 0x8) >> 3, 0, 0, 0, DT_UINT8};
		char *sol4str = formatPacket(&sol4);
		Sensor_Data sol5 = {"solenoid_5", 0, (pod_data->solenoids.ui8data & 0x20) >> 5, 0, 0, 0, DT_UINT8};
		char *sol5str = formatPacket(&sol5);
		Sensor_Data sol6 = {"solenoid_6", 0, (pod_data->solenoids.ui8data & 0x40) >> 6, 0, 0, 0, DT_UINT8};
		char *sol6str = formatPacket(&sol6);

		uart_send(sol1str);
		uart_send(sol2str);
		uart_send(sol4str);
		uart_send(sol5str);
		uart_send(sol6str);

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
