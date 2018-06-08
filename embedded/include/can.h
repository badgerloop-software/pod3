#ifndef _CAN_H__
#define _CAN_H__

#include <stdint.h>
#include "system.h"
#include <stm32l4xx_hal.h>

/*****************************************************************************/
/*                                 CAN Enums                                 */
/*****************************************************************************/


CAN_HandleTypeDef hcan;

char* can_read(CAN_HandleTypeDef *hcan);
void can_send(uint16_t can_id, size_t length, uint8_t* data, CAN_HandleTypeDef *hcan);
#endif
