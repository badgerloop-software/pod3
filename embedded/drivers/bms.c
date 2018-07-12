#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"

Bms *bms;

void bms_init() {
	bms = malloc(sizeof(Bms));
	printf("initialize_bms\r\n");
	bms->packCurrent = 0;
	bms->packVoltage = 0;
	bms->packDCL = 0;
	bms->packCCL = 0;
	bms->packResistance = 0;
	bms->packHealth = 0;
	bms->packOpenVoltage = 0;
	bms->packCycles = 0;
	bms->packAh = 0;
	bms->inputVoltage = 0;
	bms->Soc = 0;
	bms->relayStatus = 0;

	bms->highTemp = 0;
	bms->lowTemp = 0;
	bms->cellMaxVoltage = 0;
	bms->cellMinVoltage = 0;
	bms->maxCells = 0;
	bms->numCells = 0;
}

/**
  * Recieves a CAN Message and updates global BMS_Data struct
  */
int bms_parser(uint32_t id, uint8_t *data) {
	printf("Recieved bms message\r\n");
	uint8_t offset = 0;
	switch(id) {
		case 0x6B0:
			printf("Data: %d, %d, %d, %d, %d, %d, %d, %d\r\n", data[0],
					data[1], data[2], data[3], data[4], data[5], data[6],
					data[7]);
			memcpy(&bms->packCurrent, data, sizeof(bms->packCurrent));
			offset += sizeof(bms->packCurrent);
			memcpy(&bms->packVoltage, data+offset, sizeof(bms->packVoltage));
			offset += sizeof(bms->packVoltage);
			memcpy(&bms->Soc, data+offset, sizeof(bms->Soc));
			offset += sizeof(bms->Soc);
			memcpy(&bms->relayStatus, data+offset, sizeof(bms->relayStatus));
			offset += sizeof(bms->relayStatus);

			bms->packCurrent /= 10;
			bms->packVoltage /= 10;
			bms->Soc /= 2;
			printf("V: %d\r\n", bms->packVoltage);
			printf("A: %d\r\n", bms->packCurrent);
			printf("Soc: %d\r\n", bms->Soc);
			printf("Relay: %d\r\n", bms->relayStatus);
			break;
		case 0x6B1:
			memcpy(&bms->packDCL, data, sizeof(bms->packDCL));
			offset+=sizeof(bms->packDCL);
			offset += 2; // 2 bytes empty
			memcpy(&bms->highTemp, data+offset, sizeof(bms->highTemp));
			offset += sizeof(bms->highTemp);
			memcpy(&bms->lowTemp, data+offset, sizeof(bms->lowTemp));
			break;
		case 0x653:
			memcpy(&bms->relayStatus, data, sizeof(bms->relayStatus));
			offset+=sizeof(bms->relayStatus);
			memcpy(&bms->inputVoltage, data+offset, sizeof(bms->inputVoltage));
			offset += sizeof(bms->inputVoltage);

			break;
		case 0x999://0x652: //TODO 
			memcpy(&bms->packDCL, data, sizeof(bms->packDCL));
			offset+=sizeof(bms->packDCL);
			offset += 2; // 2 bytes empty
			memcpy(&bms->highTemp, data+offset, sizeof(bms->highTemp));
			offset += sizeof(bms->highTemp);
			memcpy(&bms->lowTemp, data+offset, sizeof(bms->lowTemp));
			break;
		case 0x998: //TODO
			memcpy(&bms->packDCL, data, sizeof(bms->packDCL));
			offset+=sizeof(bms->packDCL);
			offset += 2; // 2 bytes empty
			memcpy(&bms->highTemp, data+offset, sizeof(bms->highTemp));
			offset += sizeof(bms->highTemp);
			memcpy(&bms->lowTemp, data+offset, sizeof(bms->lowTemp));
			break;
		case 0x997: //TODO
			memcpy(&bms->packDCL, data, sizeof(bms->packDCL));
			offset+=sizeof(bms->packDCL);
			offset += 2; // 2 bytes empty
			memcpy(&bms->highTemp, data+offset, sizeof(bms->highTemp));
			offset += sizeof(bms->highTemp);
			memcpy(&bms->lowTemp, data+offset, sizeof(bms->lowTemp));
			break;
		default:
			printf("BMS CAN command not found 0x%08lx\r\n", id);

	}
	return 1;
}
