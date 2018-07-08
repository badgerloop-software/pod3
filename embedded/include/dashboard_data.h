#ifndef _DASHBOARD_DATA__H__
#define _DASHBOARD_DATA__H__

#include <time.h>

#define FRESH 		0
#define NOT_FRESH 	1

typedef struct {
	char   *name;
	uint8_t data;
	time_t  timestamp;
	uint8_t freshness;
} Sensor_Data;


typedef struct {
	Sensor_Data pressure;
	Sensor_Data temperature;	
} Pod_Data_Handle;


void send_data(Pod_Data_Handle*);
char *formatPacket(Sensor_Data*);
#endif
