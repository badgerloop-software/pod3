#ifndef NAV_DATA__H
#define NAV_DATA__H

#include "solenoids.h"

typedef struct {
	Solenoid_States   solenoids;
	/*Pressure_Values line_pressures;*/
	/*Retro_Values    retros;*/
	/*Motion_Values   motion;*/
} Nav_Data

int nav_DAQ(Solenoid_States*);

#endif
