#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"

#define SEND_BUF_SIZE 128

char packetBuffer[SEND_BUF_SIZE];

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor;
	if (pod_data->pressure.freshness == FRESH) {
		//pod_data->pressure.freshness = NOT_FRESH;
		sensor = &(pod_data->pressure);
		char *dataToSend = formatPacket(sensor);
		//printf("%s", dataToSend);
		uart_send(dataToSend);	
	}

	if (pod_data->temperature.freshness == FRESH) {
		//pod_data->temperature.freshness = NOT_FRESH;
		sensor = &(pod_data->temperature);
		char *dataToSend = formatPacket(sensor);
		//printf("%s\r\n", dataToSend);
		uart_send(dataToSend);
	}
}

char *formatPacket(Sensor_Data *sensorData) {
	char *sensName;
	uint8_t sensData;
	int nChars;
	/* grab the values we want to send */
	sensName = sensorData->name;
        /* print the content to the buffer */
	/* the format string will be different by type */
	nChars = -1; 	// To indicate an error
	switch(sensorData->dtype) {
		case DT_UINT8:
			sensData = sensorData->ui8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, sensData);
			break;
		case DT_UINT16:
			sensData = sensorData->ui16data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, sensData);
			break;

		case DT_INT8:
			sensData = sensorData->i8data;
			nChars = snprintf(packetBuffer, SEND_BUF_SIZE, "$|%s,%u\n", sensName, sensData);
			break;
	}

	if (nChars < 0 || nChars > SEND_BUF_SIZE) {
	        /* TODO this means our print failed or was too big */
	        /* TODO should do something smarter here */
	        printf("Problem encountered while creating sensor data payload\r\n");
	}
	return packetBuffer;
}
