#ifndef _CAN__H__
#define _CAN__H__

#include <stm32l4xx_hal.h>
#include <stdbool.h>
#include "board.h"
#include "stm32l4xx_hal_conf.h"

#define BADGER_CAN_ID 0x555

extern CAN_HandleTypeDef   can_handle;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t             TxData[8];
extern uint8_t             RxData[8];
extern uint32_t            TxMailbox;

HAL_StatusTypeDef can_read(void);
HAL_StatusTypeDef can_send(uint32_t id, uint32_t TxMailbox, size_t length, uint8_t *TxData);
HAL_StatusTypeDef can_send_intermodule(BOARD_ROLE sending_board, RECEIVING_BOARD receiving_board, uint8_t message_num, uint8_t *data) ;
HAL_StatusTypeDef can_listen(void);
HAL_StatusTypeDef board_telemetry_send(BOARD_ROLE board_type);
void print_incoming_can_message(uint32_t id, uint8_t *data);
HAL_StatusTypeDef can_init(BOARD_ROLE role);

#endif
