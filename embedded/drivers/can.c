#include <stdio.h>
#include <string.h>
#include <board.h>
#include "can.h"

CAN_HandleTypeDef can_handle;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
extern uint8_t board_num;

HAL_StatusTypeDef can_init(void){
	HAL_StatusTypeDef retval;	
	CAN_FilterTypeDef sFilterConfig0;
	/* CAN */
	HAL_Init();
	/* Clock Enables */
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/* General CAN Init */
	can_handle.Instance = CAN1;
	can_handle.Init.TimeTriggeredMode = DISABLE;
	can_handle.Init.AutoBusOff = DISABLE;
	can_handle.Init.AutoWakeUp = ENABLE;
	can_handle.Init.AutoRetransmission = ENABLE;
	can_handle.Init.ReceiveFifoLocked = DISABLE;
	can_handle.Init.TransmitFifoPriority = DISABLE;
	can_handle.Init.Mode = CAN_MODE_NORMAL;

	/* CAN Bit Timing Register Init */
	can_handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	can_handle.Init.TimeSeg1 = CAN_BS1_4TQ;
	can_handle.Init.TimeSeg2 = CAN_BS2_3TQ;
	can_handle.Init.Prescaler = 12;
	/* Prescaler Calc 
	 * Figure 452 Page 1369 of the Reference manual
	 *
	 *  Baudrate = 1/Nominal_Bit_Time
	 * Baudrate = 250kbs Tpclk = 1/24000kHz
	 * (Decided Nominal_Bit_Time = 8 Time quanta)
	 * Solve for length of one time quanta: 
	 * Tq = (Prescaler + 1) * Tpclk
	 *
	 */

	/* CAN Filter Config */
	sFilterConfig0.FilterBank = 0;
	sFilterConfig0.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig0.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig0.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig0.SlaveStartFilterBank = 14;
	
	/* Board Specific (Filter) Initialization */
	/* For CAN ID filtering, look at: https://community.st.com/thread/28968 */
	/*
	 * For an incomming CAN_ID:
	 * Bits [11:8] determine the FROM node
	 * Bits [7:4] determine the TO node(s)
	 * Bits [3:0] determine the message number
	 *
	 * Module specific filter configuration (Bits [7:4] must accept these values, others X):
	 * DASH: 	1,4,5,6,8		 	
	 * NAV:		2,5,7,8
	 * PV:		3,4,6,7,8
	 * DEV:		ALL
	 *
	 */

	if(board_num == DASH){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
		sFilterConfig0.FilterMaskIdHigh = 0x0000;
		sFilterConfig0.FilterMaskIdLow = 0x0000;
	} else if (board_num == NAV){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
		sFilterConfig0.FilterMaskIdHigh = 0x0000;
		sFilterConfig0.FilterMaskIdLow = 0x0000;
	} else if (board_num == PV){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
		sFilterConfig0.FilterMaskIdHigh = 0x0000;
		sFilterConfig0.FilterMaskIdLow = 0x0000;
	} else if (board_num == DEV){ //Read all messages
		sFilterConfig0.FilterIdHigh = 0x7ff << 5;
		sFilterConfig0.FilterIdLow = 0x0000;
		sFilterConfig0.FilterMaskIdHigh = 0x7FF << 5;
		sFilterConfig0.FilterMaskIdLow = 0x0000;
	} else{
		return HAL_ERROR;
	}
	
	sFilterConfig0.FilterActivation = ENABLE;
	
	/* Calling CAN Init Functions */
	retval = HAL_CAN_Init(&can_handle);
	if(retval != HAL_OK) return retval;

	retval = HAL_CAN_ConfigFilter(&can_handle, &sFilterConfig0);
	if(retval != HAL_OK )return retval;

	retval = HAL_CAN_Start(&can_handle);
	if(retval != HAL_OK) return retval; 	
	return retval;
}