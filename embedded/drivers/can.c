#include <stdio.h>
#include <string.h>
#include <board.h>
#include <can.h>

CAN_HandleTypeDef can_handle;
CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
extern uint8_t board_num;

uint32_t can_message_available(uint32_t RxFifo) {
	return HAL_CAN_GetRxFifoFillLevel(&can_handle, RxFifo);
}

HAL_StatusTypeDef can_send(
	uint32_t id, uint32_t TxMailbox, size_t length, uint8_t *data
) {
	HAL_StatusTypeDef retval = HAL_ERROR;

	TxHeader.StdId = id;

	/* TODO: why are we doing this? */
	if (length % 2 == 1) {
		TxHeader.DLC = (length / 2) + 1;
	} else {
		TxHeader.DLC = length / 2;
	}
	
	if (HAL_CAN_GetTxMailboxesFreeLevel(&can_handle)) {
		printf("SENDING MESSAGE\r\n");
		retval = HAL_CAN_AddTxMessage(&can_handle, &TxHeader, data, &TxMailbox);
		if (retval != HAL_OK)
			return retval;
	} else return HAL_ERROR;

	return retval;
}

HAL_StatusTypeDef can_read(void) {

	int i;
	HAL_StatusTypeDef retval = HAL_ERROR;

	if (can_message_available(CAN_RX_FIFO0)) {
		printf("CAN Message Received.\r\n");
		retval = HAL_CAN_GetRxMessage(&can_handle, CAN_RX_FIFO0, &RxHeader, RxData);
		           
		/* Printing out received data */
		printf("Received CAN ID: #%lx\r\n", RxHeader.StdId );
		for (i = 0; i < 8; i++) {
			if (RxData[i] != 0) {
				printf("CAN Message Data[%d]: %x\r\n", i, RxData[i]);
			}
		}
	}
	return retval;
}

HAL_StatusTypeDef can_send_intermodule(
		BOARD_ROLE sending_board, RECEIVING_BOARD receiving_board, uint8_t message_num, uint8_t *data){
	HAL_StatusTypeDef retval = HAL_ERROR;
	
	/* Generate CAN ID */
	uint32_t byte1_id = 0;
	byte1_id = sending_board << 6;
	byte1_id |= (receiving_board << 3);
	byte1_id |= (message_num);
	can_send(BADGER_CAN_ID, 0, 8, data);
	return retval;

}

HAL_StatusTypeDef can_init(BOARD_ROLE role) {

	HAL_StatusTypeDef retval;	
	CAN_FilterTypeDef sFilterConfig0;

	/* Clock Enables */
	__HAL_RCC_CAN1_CLK_ENABLE();

	/* General CAN Init */
	can_handle.Instance						= CAN1;
	can_handle.Init.TimeTriggeredMode		= DISABLE;
	can_handle.Init.AutoBusOff				= DISABLE;
	can_handle.Init.AutoWakeUp				= ENABLE;
	can_handle.Init.AutoRetransmission		= ENABLE;
	can_handle.Init.ReceiveFifoLocked		= DISABLE;
	can_handle.Init.TransmitFifoPriority	= DISABLE;
	can_handle.Init.Mode					= CAN_MODE_NORMAL;

	/* CAN Bit Timing Register Init */
	can_handle.Init.SyncJumpWidth			= CAN_SJW_1TQ;
	can_handle.Init.TimeSeg1				= CAN_BS1_4TQ;
	can_handle.Init.TimeSeg2				= CAN_BS2_3TQ;

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
	can_handle.Init.Prescaler				= 12;

	/* Initialize the transmit header to values we'll always use */
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.TransmitGlobalTime = DISABLE;

	/* CAN Filter Config */
	sFilterConfig0.FilterBank				= 0;
	sFilterConfig0.FilterMode				= CAN_FILTERMODE_IDMASK;
	sFilterConfig0.FilterScale				= CAN_FILTERSCALE_32BIT;
	sFilterConfig0.FilterFIFOAssignment		= CAN_RX_FIFO0;
	sFilterConfig0.SlaveStartFilterBank		= 14;
	
	/* Board Specific (Filter) Initialization
	 *
	 * For CAN ID filtering, look at: https://community.st.com/thread/28968
	 *
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
	
	switch (role) {
		case DASH:
			sFilterConfig0.FilterIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh 	= 0x7ff << 5;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case NAV:
			sFilterConfig0.FilterIdHigh		= 0x0000;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x0000;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case PV:
			sFilterConfig0.FilterIdHigh		= 0x0000;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x0000;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case DEV:
			sFilterConfig0.FilterIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		default:
			return HAL_ERROR;
	}
	
	sFilterConfig0.FilterActivation = ENABLE;
	
	/* Calling CAN Init Functions */
	retval = HAL_CAN_Init(&can_handle);
	if (retval != HAL_OK) return retval;

	retval = HAL_CAN_ConfigFilter(&can_handle, &sFilterConfig0);
	if (retval != HAL_OK) return retval;

	retval = HAL_CAN_Start(&can_handle);
	if (retval != HAL_OK) return retval; 	
	return retval;
}
