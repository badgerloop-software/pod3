#ifndef _RETRO__H__
#define _RETRO__H__

#include "exti.h"

#define RETRO1 		interLine[0]
#define RETRO2 		interLine[1]
#define RETRO3		interLine[5]

#define LIM1        interLine[3]
#define LIM2        interLine[6]
#define LIM3        interLine[7]

#define MAINFILTERINDEX 	(mainRetro->count % AVERAGE_SIZE)
#define MAIN_INTERVAL       (mainRetro->filter[MAINFILTERINDEX])

timeStamp *mainRetro;
int tape_vel;
void initRetro(void); 

// Sets the position in cm and velocity in cm/s
int getRetroTelemetry (int *retro_count, int *pos, int *vel, int *acc);

#endif
