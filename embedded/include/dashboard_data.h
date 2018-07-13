#ifndef _DASHBOARD_DATA__H__
#define _DASHBOARD_DATA__H__

#include <time.h>

#define FRESH 		0
#define NOT_FRESH 	1

typedef enum {
        DT_UINT8 = 0,
        DT_UINT16,
        DT_INT8
} sensor_dtype;

/* TODO some sensors have different data types */
/* TODO could also just keep an int and translate as needed */
typedef struct {
	char     *name;
	int8_t   i8data;
	uint8_t  ui8data;
	uint16_t ui16data;
	time_t   timestamp;
	uint8_t  freshness;
    sensor_dtype dtype;
} Sensor_Data;


typedef struct {
	Sensor_Data current_pressure;
	Sensor_Data lv_battery_temp;
	Sensor_Data position;
	Sensor_Data velocity;
	Sensor_Data acceleration;
	Sensor_Data tube_pressure;	
} Pod_Data_Handle;


void send_data(Pod_Data_Handle*);
char *formatPacket(Sensor_Data*);
int isRightTime(unsigned int interval)
void harvest_honeywell(Pod_Data_Handle*);
#endif
