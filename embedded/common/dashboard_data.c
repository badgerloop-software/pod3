#include <stdlib.h>
#include <string.h>
#include "dashboard_data.h"
#include "uart.h"

#define SEND_BUFFER_SIZE 100

void send_data(Pod_Data_Handle *pod_data) {
	Sensor_Data *sensor = NULL;
	if (pod_data->pressure.freshness == FRESH) {
		pod_data->pressure.freshness = NOT_FRESH;
		sensor = &(pod_data->pressure);
		char *dataToSend = formatPacket(sensor);
		printf("%s", dataToSend);	
	}

	if (pod_data->temperature.freshness == FRESH) {
		pod_data->temperature.freshness = NOT_FRESH;
		sensor = &(pod_data->temperature);
		char *dataToSend = formatPacket(sensor);
		printf("%s", dataToSend);
	}
}

char *formatPacket(Sensor_Data *sensorData) {
	uint8_t data = sensorData->data;
	char packetBuffer[SEND_BUFFER_SIZE];
	int arrIter = 0;
	packetBuffer[arrIter++] = '$', packetBuffer[arrIter++] = '|'; // Set up the crazy encoding
	
	size_t i;
	size_t nameLength = strlen(sensorData->name);

	// Add the name to the message
	for (i = 0; i < nameLength; i++) {
		if (sensorData->name[i] == '\0') {
			break;
		}
		packetBuffer[arrIter++] = sensorData->name[i];
	}
	packetBuffer[arrIter++] = ':';

	char dataBuff[20];
	sprintf(dataBuff, "%d",data);
	size_t dataLength = strlen(dataBuff);
	// Add the data to the message
	for (i = 0; i <= dataLength; i++) {
		packetBuffer[arrIter++] = dataBuff[i];
	}

	char *packet = NULL;
	strcpy(packet, packetBuffer);
	return packet;
}
