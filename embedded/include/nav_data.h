#ifndef NAV_DATA__H
#define NAV_DATA__H

#include "solenoid.h"
#include <stdint.h>

typedef struct {
	uint8_t topRetroCount;
    uint8_t middleRetroCount;
	uint8_t bottomRetroCount;
	uint8_t retroAgreement;
} Retro_Values;

typedef struct {
	uint8_t limitValue1;
    uint8_t limitValue2;
	uint8_t limitValue3;
} Limit_Switches;

typedef struct {
	int8_t posX;
	int8_t velX;
	int8_t accX;
	int8_t accY;
	int8_t accZ;
} Motion_Values;

typedef struct {
	uint16_t pres_1;
	uint16_t pres_2;
	uint16_t pres_3;
	uint16_t pres_4;
	uint16_t pres_5;
	uint16_t pres_6;
	uint16_t pres_7;
	uint16_t pres_8;
} Pressure_Values;

typedef struct {
	Solenoid_States   solenoids;
	Pressure_Values   linePressures;
	Retro_Values      retros;
	Motion_Values     motion;
    Limit_Switches    limits;
    int               shouldStop;
} Nav_Data;

int nav_DAQ(Nav_Data*);

#endif
