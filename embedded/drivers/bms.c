#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bms.h"
#include "can.h"

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

	bms_clearFaults();
}

int bms_clearFaults(void){

	uint16_t can_id = 0x7e3;
	uint8_t TxData[8];
	uint8_t length = 16;

	TxData[0] = 0x01;
	TxData[1] = 0x04;
	TxData[2] = 0x00;
	TxData[3] = 0x00;
	TxData[4] = 0x00;
	TxData[5] = 0x01;
	TxData[6] = 0x00;
	TxData[7] = 0x00;
	
	can_send(can_id, 0, length, TxData );

	return 0;
}

/**
  * Recieves a CAN Message and updates global BMS_Data struct
  */
int bms_parser(uint32_t id, uint8_t *data) {
	printf("Recieved bms message\r\n");
	uint8_t offset = 0;
	switch(id) {
		case 0x6B0:
			printf("ID: 0x%3lx\r\n", id);
			printf("Data: %d, %d, %d, %d, %d, %d, %d, %d\r\n", data[0],
					data[1], data[2], data[3], data[4], data[5], data[6],
					data[7]);
			bms->packCurrent = data[1] | data[0] << 8;
			bms->packCurrent /= 10;
			bms->packVoltage = data[3] | data[2] << 8;
			bms->packVoltage /= 10;
			bms->Soc = data[4]/2;
			bms->relayStatus = data[6] | data[5] << 8;

			printf("V: %f\r\n", bms->packVoltage);
			printf("A: %f\r\n", bms->packCurrent);
			printf("Soc: %d\r\n", bms->Soc);
			printf("Relay: %d\r\n", bms->relayStatus);
			break;
		case 0x6B1:
			printf("ID: 0x%3lx\r\n", id);
			bms->packDCL = data[1] | data[0] << 8;
			bms->highTemp = data[4];
			bms->lowTemp = data[5];
			printf("DCL: %d\r\n", bms->packDCL);
			printf("High T: %d\r\n", bms->highTemp);
			printf("Low T: %d\r\n", bms->lowTemp);

			break;
		case 0x653:
			printf("ID: 0x%3lx\r\n", id);
			
			bms->relayStatus = data[1] | data[0] << 8;
			bms->inputVoltage = data[3] | data[2] << 8;
			bms->inputVoltage /= 10;
			
			printf("Relay status %d\r\n", bms->relayStatus);
			printf("Input Source Supply Voltage: %f\r\n", bms->inputVoltage);
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

