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
		retval = HAL_CAN_GetRxMessage(&can_handle, CAN_RX_FIFO0, &RxHeader, RxData);
		           
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
	uint8_t to_from_id = 0;
	uint8_t message_id = 0;
	
	to_from_id = sending_board << 4;
	to_from_id |= (receiving_board);
	message_id = (message_num);

	data[0] = to_from_id;
	data[1] = message_id;

	retval = can_send(BADGER_CAN_ID, 0, 8, data);
	return retval;
}

HAL_StatusTypeDef board_telemetry_send(BOARD_ROLE board){
	switch (board) {
		case DASH:
			//TODO dash heartbeat
			//TODO RMS heartbeat
			return HAL_ERROR;
			break;
		case NAV:
			//TODO nav heartbeat
			//TODO nav imu,
			//TODO nav retros/velocity
			//TODO nav pressure 1
			//TODO nav pressure 2
			//TODO nav pressure 3
			//TODO nav solenoid 
			//TODO nav should_stop heartbeat
			return HAL_ERROR;
			break;
		case PV:
			//TODO pv heartbeat
			//TODO Shutdown_Circuit_Status
			//TODO PV Pressure
			return HAL_ERROR;
			break;
		case DEV:
			return HAL_ERROR;
			break;
		default:
			return HAL_ERROR;
	}
}

HAL_StatusTypeDef board_telemetry_parse(uint32_t can_id, uint8_t *data){
		BOARD_ROLE from_module = (data[0] & 0xf0) >> 4;
		RECEIVING_BOARD to_modules = data[0] & 0xf;
		CAN_MESSAGE_TYPE message_num = data[1];
		
		printf("CAN_ID: %lx FROM MODULE: %d TO MODULE: %d Message num: %d", can_id, from_module, to_modules, message_num);

		return HAL_ERROR;
}

HAL_StatusTypeDef can_listen(void){
	HAL_StatusTypeDef retval = HAL_OK;	
	if (can_message_available(CAN_RX_FIFO0)) {
		retval = HAL_CAN_GetRxMessage(&can_handle, CAN_RX_FIFO0, &RxHeader, RxData);
		print_incoming_can_message(RxHeader.StdId, RxData);
	} 
	return retval;
}

void print_incoming_can_message(uint32_t id, uint8_t *data){
	BOARD_ROLE from_module = (data[0] & 0xf0) >> 4;
	RECEIVING_BOARD to_modules = data[0] & 0xf;
	CAN_MESSAGE_TYPE message_num = data[1];

	if (id == 0x555){
		printf("CAN ID: %lx (BADGER CAN ID)\r\n", id);
	} else {
		printf("CAN ID: %lx\r\n", id);
	}

	printf("TO: 		");
	switch (to_modules) {
		case DEV_REC:
		       printf("DEV\r\n");
		       break;
	        case DASH_REC:
			printf("DASH\r\n");
			break;
	 	case NAV_REC:
			printf("NAV\r\n");
			break;
		case PV_REC:
			printf("PV\r\n");
			break;
		case CCP_NAV_REC:
			printf("CCP and NAV\r\n");
			break;
		case CCP_PV_REC:
			printf("CCP and PV\r\n");
			break;
		case NAV_PV_REC:
			printf("NAV and PV\r\n");
			break;
		case ALL:
			printf("ALL\r\n");
			break;
		default:
			printf("UNKNOWN\r\n");
	}

	printf("FROM:		");
	switch(from_module) {
		case DEV:
			printf("DEV\r\n");
			break;
		case DASH:
			printf("DASH\r\n");
			break;
		case NAV:
			printf("NAV\r\n");
			break;
		case PV:
			printf("PV\r\n");
			break;
		default:
			printf("UNKNOWN\r\n");	
	}
	printf("TYPE:		");
	switch (message_num) {
		case CAN_TEST_MESSAGE:
			printf("CAN TEST\r\n");
			break;
		default:
			printf("UNKNOWN\r\n");
			break;
	}
	printf("SIZE:	 	%d bytes\r\n", sizeof(data)/sizeof(uint8_t)*2); 

	int i;
	for (i = 0; i < 8; i++){
		printf("Data[%d]:	%x\r\n", i, data[i]);
	}
}

HAL_StatusTypeDef bms_telemetry_parse(uint32_t id, uint8_t *data){
	switch (id){
		case (BMS_PACK_STATE_MESSAGE):
			printf("data 0: %x", data[0]);
		case (BMS_PACK_TEMP_MESSAGE):

		case (BMS_RELAY_STATE_MESSAGE):

		case (BMS_PACK_CCL):

		case (BMS_CELL_VOLT_MESSAGE):

		case (BMS_SOC_MESSAGE):

//		case (BMS_PACK_CURRENT_MESSAGE):

//		case (BMS_PACK_VOLT_MESSAGE):

		default:
			return HAL_ERROR;
	}
}

HAL_StatusTypeDef can_init(BOARD_ROLE role) {

	HAL_StatusTypeDef retval;	
	CAN_FilterTypeDef sFilterConfig0;

	/* Clock Enables */
	__HAL_RCC_CAN1_CLK_ENABLE();

	/* General CAN Init */
	can_handle.Instance			= CAN1;
	can_handle.Init.TimeTriggeredMode	= DISABLE;
	can_handle.Init.AutoBusOff		= DISABLE;
	can_handle.Init.AutoWakeUp		= ENABLE;
	can_handle.Init.AutoRetransmission	= ENABLE;
	can_handle.Init.ReceiveFifoLocked	= DISABLE;
	can_handle.Init.TransmitFifoPriority	= DISABLE;
	can_handle.Init.Mode			= CAN_MODE_NORMAL;

	/* CAN Bit Timing Register Init */
	can_handle.Init.SyncJumpWidth		= CAN_SJW_1TQ;
	can_handle.Init.TimeSeg1		= CAN_BS1_4TQ;
	can_handle.Init.TimeSeg2		= CAN_BS2_3TQ;

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
	can_handle.Init.Prescaler		= 12;

	/* Initialize the transmit header to values we'll always use */
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.TransmitGlobalTime = DISABLE;

	/* CAN Filter Config */
	sFilterConfig0.FilterBank		= 0;
	sFilterConfig0.FilterMode		= CAN_FILTERMODE_IDMASK;
	sFilterConfig0.FilterScale		= CAN_FILTERSCALE_32BIT;
	sFilterConfig0.FilterFIFOAssignment	= CAN_RX_FIFO0;
	sFilterConfig0.SlaveStartFilterBank	= 14;
	
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
			sFilterConfig0.FilterMaskIdHigh 	= 0x0000;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case NAV:
			sFilterConfig0.FilterIdHigh		= 0x555 << 5;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case PV:
			sFilterConfig0.FilterIdHigh		= 0x555 << 5;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterMaskIdLow		= 0x0000;
			break;
		case DEV:
			sFilterConfig0.FilterIdHigh		= 0x7ff << 5;
			sFilterConfig0.FilterIdLow		= 0x0000;
			sFilterConfig0.FilterMaskIdHigh		= 0x0000;
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