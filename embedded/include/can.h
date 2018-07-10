#ifndef _CAN__H__
#define _CAN__H__

#include <stm32l4xx_hal.h>
#include <stdbool.h>
#include "stm32l4xx_hal_conf.h"

extern CAN_HandleTypeDef     hcan;
extern CAN_TxHeaderTypeDef   TxHeader;
extern CAN_RxHeaderTypeDef   RxHeader;
extern uint8_t               TxData[8];
extern uint8_t               RxData[8];
extern uint32_t TxMailbox;

bool can_read(void);
bool can_send(uint32_t can_id, size_t length, uint8_t *TxData);
HAL_StatusTypeDef can_init(void);

typedef enum sending_module_val{
	RMS_SEND = 0,
 	CCP_SEND = 1,
	NAV_SEND = 2,
	PV_SEND = 4,
	BMS_SEND = 7
} SENDING_MODULE;

typedef enum receiving_module_val{
	RMS_REC_1 = 0xC,
	RMS_REC_2 = 0xA,
	CCP_REC = 1,
	NAV_REC = 2,
	PV_REC = 3,
	BMS_REC = 0xe,
	ALL_REC = 4,
	CCP_NAV_REC = 5,
	CCP_PV_REC = 6,
	NAV_PV_REC = 7,
	CCP_NAV_PV_REC =8
} RECEIVING_MODULE;

typedef enum message_type {
	CCP_FAULT = 0,
	CCP_WARNING = 1,
	LV_HEARTBEAT = 2,
	NAV_SOLENOID_COMMAND = 3,
	PV_MCU_ENABLE_COMMAND = 4,
	PV_FAULT =5,
	PV_WARNING =6,
	SHUTDOWN_CIRCUIT_STATUS=7,
	PV_PRESSURE=8,
	// RMS_ENABLE_HEARTBEAT
	// RMS_CLEAR_FAULTS
	// RMS_TORQUE
	// RMS_DISABLE_INVERTER
	// RMS_CAP_DISCHARGE
	NAV_FAULT=9,
	NAV_WARNING=10,
	TAPE_SHOULD_STOP=11,
	NAV_PRES_1=12,
	NAV_PRES_2=13,
	NAV_PRES_3=14,
	NAV_SOLENOID_1=15
	//OBDII_RETURN
} MESSAGE_TYPE;

#endif
