#ifndef _CAN__H__
#define _CAN__H__

#include <stm32l4xx_hal.h>
#include <stdbool.h>
#include "board.h"
#include "stm32l4xx_hal_conf.h"
#include "dashboard_data.h"

#define BADGER_CAN_ID 0x555

extern CAN_HandleTypeDef   can_handle;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t             TxData[8];
extern uint8_t             RxData[8];
extern uint32_t            TxMailbox;
extern uint32_t post_run_transition;

typedef enum{
    IDLE_MSG,           //Default message, sends all 0's
    CLEAR_FAULTS_MSG,   //Clears CAN faults
    FAULTS_CLEARED, //Cleared CAN faults on RMS
    PRE_RUN,        // Same message as IDLE, but represents ready-to-run state
    FORWARD,        //Commands motor forward
    SPIN_DOWN,      //Same message as IDLE, but represents slowing down
    DISCHARGE,      //Discharging capacitors after the run
    POST_RUN_MSG,       //Also same as IDLE, but represents post run
    FAULT           //Also same as IDLE, but represents falt state
}heartbeat_msg_t;

int can_heartbeat_idle( CAN_HandleTypeDef *hcan );
int can_heartbeat_clear_faults( CAN_HandleTypeDef *hcan );
int can_heartbeat_forward( CAN_HandleTypeDef *hcan );
int can_heartbeat_discharge( CAN_HandleTypeDef *hcan );
int can_heartbeat_handler( CAN_HandleTypeDef *hcan );
void can_heartbeat_next(void);
void can_heartbeat_fault(void);

HAL_StatusTypeDef can_read(void);
HAL_StatusTypeDef can_send(uint32_t id, uint32_t TxMailbox, size_t length, uint8_t *TxData);
HAL_StatusTypeDef can_send_intermodule(BOARD_ROLE sending_board, RECEIVING_BOARD receiving_board, uint8_t message_num, uint8_t *data) ;
HAL_StatusTypeDef can_listen(void);
HAL_StatusTypeDef board_telemetry_send(BOARD_ROLE board_type);
HAL_StatusTypeDef ccp_parse_can_message(uint32_t can_id, uint8_t *data, Pod_Data_Handle *pod_data);
HAL_StatusTypeDef board_can_message_parse(uint32_t can_id, uint8_t *data);
void print_incoming_can_message(uint32_t id, uint8_t *data);
HAL_StatusTypeDef can_init(BOARD_ROLE role);

#endif
