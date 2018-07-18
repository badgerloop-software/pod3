#ifndef _BMS_H_
#define _BMS_H_

#include <stdint.h>

typedef struct {
	float packCurrent;
	float packVoltage;
	uint16_t packDCL;
	uint16_t packCCL;
	uint16_t packResistance;
	uint8_t packHealth;
	float packOpenVoltage;
	uint16_t packCycles;
	uint16_t packAh;
	float inputVoltage;
	uint8_t Soc;
	uint16_t relayStatus;

	uint8_t highTemp;
	uint8_t lowTemp;
	uint16_t cellMaxVoltage;
	uint16_t cellMinVoltage;
	uint16_t cellAvgVoltage;
	uint8_t maxCells;
	uint8_t numCells;
} Bms;

Bms *bms;
void bms_init();
int bms_clearFaults();
int bms_parser(uint32_t id, uint8_t *data);

#endif
