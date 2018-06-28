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
	//TODO val is a placeholder
	uint8_t val = 0;
	uint8_t temp = -1;
	printf("val %d", val);
	printf("temp %d", temp);


//#if CCP_MODULE
	SET_VBATT(val);
	SET_IBATT(val);
	SET_TBATT(val);
	SET_TPOD(val);
	SET_TUBEPRESSURE(val);

//#endif

//#if NAV_MODULE
	
	//Pneumatic pressure
	SET_PRES_TANK_PRI(val);
#if DEBUG_IO
	printf("primary tank pressure %d", val);
#endif
	SET_PRES_TANK_SEC(val);
	
#if DEBUG_IO
	printf("secondary tank pressure %d", val);
#endif
	SET_PRES_LINE_PRI(val);
	
#if DEBUG_IO
	printf("primary line pressure %d", val);
#endif
	SET_PRES_LINE_SEC(val);
	
#if DEBUG_IO
	printf("secondary line pressure %d", val);
#endif
	SET_PRES_ACTU_PRI(val);
	
#if DEBUG_IO
	printf("primary actuator  pressure %d", val);
#endif
	SET_PRES_ACTU_SEC(val);
	
#if DEBUG_IO
	printf("secondary actuator pressure %d", val);
#endif
	
	
	
	
	//Accel, Velocity, Position
	SET_ACCEL_x(val);
	SET_ACCEL_y(val);
	SET_ACCEL_z(val);
	SET_POS(val);
	SET_VEL(val);

	//LIMIT
	temp = gpio_readPin(GPIOA, 3);
	SET_DLIM;
	temp = gpio_readPin(GPIOA, 6);
	SET_BLIM1;
	temp = gpio_readPin(GPIOA, 7);
	SET_BLIM2;


//#endif

//#if PV_MODULE
	

	temp = gpio_readPin(GPIOB, 0);
	if(temp) SET_MSTR_SW_FDBK;
	else CLR_MSTR_SW_FDBK;

	temp = gpio_readPin(GPIOB, 7);
	if(temp) SET_E_STOP_FDBK;
	else CLR_E_STOP_FDBK;

	temp = gpio_readPin(GPIOB, 6);
	if(temp) SET_PV_LIM_FDBK;
	else CLR_PV_LIM_FDBK;

	temp = gpio_readPin(GPIOB, 1);
	if(temp) SET_HVD_FDBK;
	else CLR_HVD_FDBK;

	temp = gpio_readPin(GPIOC, 14);
	if(temp) SET_BMS_STAT_FDBK;
	else CLR_BMS_STAT_FDBK;

	temp = gpio_readPin(GPIOC, 8);
	if(temp) SET_IMD_STAT_FDBK;
	else CLR_IMD_STAT_FDBK;
	
	temp = gpio_readPin(GPIOA, 8);
	if(temp) SET_HV_EN_FDBK;
	else CLR_HV_EN_FDBK;




//#endif


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

