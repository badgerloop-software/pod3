#ifndef _CAN_H__
#define _CAN_H__

#include <stdint.h>
#include "system.h"
#include <stm32l4xx_hal.h>
#include "badgerloop.h"
/*****************************************************************************/
/*                                 CAN Enums                                 */
/*****************************************************************************/

CAN_HandleTypeDef hcan;
//uint8_t volatile hb_torque = 0;
uint8_t message_num;

typedef enum{
	IDLE,           //Default message, sends all 0's
	FAULTS_CLEARED, //Clears CAN faults on RMS
	PRE_RUN,        // Same message as IDLE, but represents ready-to-run state
    FORWARD,        //Commands motor forward
    SPIN_DOWN,      //Same message as IDLE, but represents slowing down
	DISCHARGE,      //Discharging capacitors after the run
    POST_RUN        //Also same as IDLE, but represents post run
}heartbeat_msg_t;

//volatile heartbeat_msg_t hb_status = IDLE;

int can_read(CAN_HandleTypeDef *hcan);
int can_send(uint32_t can_id, size_t length, uint8_t* data, CAN_HandleTypeDef *hcan);
uint8_t * can_send_obd2(uint16_t can_id, size_t length, uint8_t mode,uint16_t pid, CAN_HandleTypeDef *hcan);
int intermodule_can_message(SENDING_MODULE sending_module, RECEIVING_MODULE receiving_module, int message_num, MESSAGE_TYPE message_type, uint8_t *TxData, CAN_HandleTypeDef *hcan);

/* Heartbeat Related */
int can_heartbeat_handler( CAN_HandleTypeDef *hcan);
int can_heartbeat_clear_faults( CAN_HandleTypeDef *hcan );
int can_heartbeat_idle( CAN_HandleTypeDef *hcan );
int can_heartbeat_forward( CAN_HandleTypeDef *hcan );
int can_heartbeat_discharge( CAN_HandleTypeDef *hcan );
int can_update( uint8_t message_number);

#endif
