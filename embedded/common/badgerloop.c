#include "badgerloop.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "gpio.h"

#define DEBUG_IO	1

uint32_t DONT_BRAKnt32_t, DONT_BRAKE_TO = 8000, MUST_BRAKE_TO = 30000, BRAKING_COUNT_THRS = 5, ACCEL_IMPULSE = 100, TARGET_END_POS = 125000, CM_PER_STRIP = 3048, INTERPOLATE = 10;

/*****************************************************************************/
/*                            Get Telemetry Vals                            */
/*****************************************************************************/
int bms_software_reset_set( bool state ){
    
    //PB Reset is on Pin4
    if ( gpio_writePin( GPIOB, 4, state ) == -1 ){
        printf( "BMS Software Reset Set Error\r\n");
        return -1;
    }
    
    return 0;

}

void battery_votlage(void) {
	//Analog read and scaling
	SET_VBATT(15000);	//TODO update with read
}

int thermistor_scalar(uint16_t reading){
	float retval = (float) reading;
	float To = 60.0 + 273.0;
	float Ro = 2760.0; //at 60 Celsius the resitance is 2760 ohms
	float B = 3435.0;

	/* volts */
	retval = reading * (3.3 / 1024.0);
	/* ohm */
	retval = (33000.0 / retval)  - 10000.0;
	/* 1 / retval */
	retval = (1.0 / To) + ((1.0 / B) * log(retval / Ro));
	return (int) ((1 / retval) - 273);

}

void telemetry_setup(void) {


}

int calculate_stopping_distance(int velocity, int target_decel){
/* must be moving forward, want to move backward */
	if (velocity <= 0 || target_decel >= 0)
		return TARGET_END_POS;
	velocity *= velocity;
	return -(velocity / target_decel);
}

/*****************************************************************************/
/*                            Actuation Functions                            */
/*****************************************************************************/
static int primary_intensity = -1, secondary_intensity = -1;

void primary_brakes(int intensity){
#if DEBUG_IO

	printf("primary brakes %d %s", intensity, intensity ? "not driven" : "driven"); 
#endif
	
	if(intensity != primary_intensity){
		//TODO call I2C actuation function
	}
	primary_intensity = intensity;
}

void secondary_brakes(int intensity){
#if DEBUG_IO

	printf("secondary brakes %d %s", intensity, intensity ? "not driven" : "driven"); 
#endif
	
	if(intensity != secondary_intensity){
		//TODO call I2C actuation function 
	}
	secondary_intensity = intensity;
}

void vent_primary_brakes(bool open) {
#if DEBUG_IO
	printf("vent primary brakes %s\r\n", open ? "not driven" : "driven");
#endif
	// call I2C Vent function
	
}

void vent_secondary_brakes(bool open) {
#if DEBUG_IO
	printf("vent secondary brakes %s\r\n", open ? "not driven" : "driven");
#endif
	// call I2C Vent function
}
