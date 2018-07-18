#ifndef _DASHBOARD_DATA__H__
#define _DASHBOARD_DATA__H__

#include <time.h>
#include "rms.h"
#include "bms.h"
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
	Sensor_Data retro;
	Sensor_Data solenoids;
	Sensor_Data linePressures[8];
	Sensor_Data BMSdata[19];
	Sensor_Data RMSdata[22];
} Pod_Data_Handle;


void set_accel_vel_pos(Pod_Data_Handle*, int8_t, int8_t, int8_t);
void package_bms_data(Pod_Data_Handle*, Bms*);
void package_rms_data(Pod_Data_Handle*, Rms*);
void set_pres_1_2(Pod_Data_Handle*, uint16_t, uint16_t);
void set_pres_3_4(Pod_Data_Handle*, uint16_t, uint16_t);
void set_pres_5_6(Pod_Data_Handle*, uint16_t, uint16_t);
void set_pres_7_8(Pod_Data_Handle*, uint16_t, uint16_t);
void set_solenoid_value(Pod_Data_Handle*, uint8_t);
void set_retro(Pod_Data_Handle*, uint8_t);
int dash_DAQ(Pod_Data_Handle *);
void send_data(Pod_Data_Handle*);
char *formatPacket(Sensor_Data*);
void harvest_honeywell(Pod_Data_Handle*);
#endif
