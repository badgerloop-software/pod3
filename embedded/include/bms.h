#ifndef _BMS_H_
#define _BMS_H_

#include <stdint.h>

typedef struct {
	short packCurrent;
	short packVoltage;
	short packDCL;
	short packCCL;
	short packResistance;
	short packHealth;
	short packOpenVoltage;
	short packCycles;
	short packAh;
	short inputVoltage;
	uint8_t Soc;
	uint16_t relayStatus;

	uint8_t highTemp;
	uint8_t lowTemp;
	short cellMaxVoltage;
	short cellMinVoltage;
	short cellAvgVoltage;
	uint8_t maxCells;
	uint8_t numCells;
} Bms;

extern Bms *bms;
void bms_init();
int bms_parser(uint32_t id, uint8_t *data);

#endif
