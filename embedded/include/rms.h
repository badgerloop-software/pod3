#ifndef _RMS_H__
#define _RMS_H__

#include <stdint.h>
#include <stdbool.h>
#include "commands.h"

/*****************************************************************************/
/*                                 RMS Enums	                             */
/*****************************************************************************/



typedef enum {
	SET_TORQUE_MODE = 0x0AA,
	CLEAR_FAULTS = 0x0C1,
	ENABLE_HEARTBEAT = 0x0C1,
	DISABLE_INVERTER = 0x0C0,
	ENABLE_INVERTER = 0x0C0,
	TORQUE_0 = 0x0C0,
	TORQUE_10 = 0x0C0,
	TORQUE_20 = 0x0C0,
	TORQUE_30 = 0x0C0,
	TORQUE_40 = 0x0C0,
	TORQUE_50 = 0x0C0,
	TORQUE_60 = 0x0C0,
	TORQUE_70 = 0x0C0,
	TORQUE_80 = 0x0C0,
	TORQUE_90 = 0x0C0,
	TORQUE_100 = 0x0C0,
	MULTI_DISABLE_INVERTER = 0x0C0,
	RPM_THRES_DISABLE = 0x0C0,	

} RMS_COMMAND_ID;
/*
 * typedef enum {
 * SET_TORQUE_MODE = 0x00800000000004,
 * CLEAR_FAULTS = 0x00 ,
 * ENABLE_HEARTBEAT = 0x00,
 * DISABLE_INVERTER = 0x00000000000000,
 * ENABLE_INVERTER = 0x00,
 * TORQUE_0 = 0x00,
 * TORQUE_10 = 0x00,
 * TORQUE_20 = 0x00,
 * TORQUE_30 = 0x00,
 * TORQUE_40 = 0x00,
 * TORQUE_50 = 0x00,
 * TORQUE_60 = 0x00,
 * TORQUE_70 = 0x00,
 * TORQUE_80 = 0x00,
 * TORQUE_90 = 0x00,
 * TORQUE_100 = 0x00,
 * MULTI_DISABLE_INVERTER = 0x00,
 * RPM_THRES_DISABLE = 0x00,
} RMS_COMMANDS_BYTE;
 */
typedef enum {
	STATE = 0x0AA,
	FAULTS = 0x0AB,
	LV_BATT = 0x0A9,
	HV_BATT = 0x0A7,
	CHECK_FAULTS = 0x0C2,
	SPEED = 0x0A5,
	CURRENT = 0x0A6,
	TORQUE = 0x0AC,
	//TODO get values from wangnan for other attributes
	RPM = 0x000,
	TEMP = 0x000,
} RMS_QUERY_ID;
/*	
typedef enum { 
	STATE = 0x00,
	FAULTS = 0x00,
	LV_BATT = 0x00,
	HV_BATT = 0x00,
	CHECK_FAULTS = 0x00,
	SPEED = 0x00,
	CURRENT = 0x00,
	TORQUE = 0x00,
	RPM = 0x00,
	TEMP = 0x00,
} RMS_QUERY_BYTE;
*/

/*****************************************************************************/
/*				Functions				     */
/*****************************************************************************/
/*				Setters 				     */
int rms_clearFaults();
int rms_enableHeartbeat();
int rms_disableInverter();
int rms_enableInverter();
int rms_setTorque(uint8_t val);
int rms_multDisableInverter();
int rms_rpmThresDisable();
/*				Getters					     */
char* rms_getState(void);
uint8_t rms_getLockoutEnabled(void);
char* rms_getFaults(void);
int32_t rms_getLVVolt(void);
int32_t rms_getHVVolt(void);
uint16_t rms_checkFaults(void);
int32_t rms_getSpeed(void);
int32_t rms_getCurrent(void);
int32_t rms_getTorque(void);
int32_t rms_getRPM(void);
#endif
