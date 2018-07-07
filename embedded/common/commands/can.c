#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "can.h"

extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t RxData[8];
extern uint8_t TxData[8];
extern CAN_HandleTypeDef can_handle;

command_status can_try_init(void){
	return(can_init() == HAL_OK) ? CMD_SUCCESS: FAIL;
}

//TODO
command_status try_send(uint32_t can_id, size_t length, uint8_t *TxData){
	if(can_send(can_id, length, TxData) != HAL_OK)
		return FAIL;

	return CMD_SUCCESS;	
}
command_status can_try_read(void){
	HAL_StatusTypeDef retval = FAIL;
	if(HAL_CAN_GetRxFifoFillLevel(&can_handle, CAN_RX_FIFO0)){
		retval = HAL_CAN_GetRxMessage(& can_handle, CAN_RX_FIFO0, &RxHeader, RxData);
		           
		/* Printing out received data */
		printf("Received CAN ID: #%lx\r\n", RxHeader.StdId );
		uint8_t i;
		for(i = 0; i < 8; i++){
			if( RxData[i] != 0){
				printf("CAN Message Data #%d: %x\r\n", i, RxData[i]);
			}
		}
	}
	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;	
}

command_status do_can(int argc, char *argv[]) {
	printf("%s: TODO (%d args given)\r\n", argv[0], argc);
	
	if(argc == 1) return USAGE;

	/* init (or re-init) the can subsystem */
	if(!strcmp("init", argv[1]))
		return can_try_init();
	/* read a can message */
	if(!strcmp("read", argv[1]))
		return can_try_read();
	
	//if(argc == 2 || argc == 3 || argc == 4) return USAGE;

	/* send a can message */
	
	if(!strcmp("send", argv[1])){
		//TEST MESSAGE SEND
		uint32_t can_id = 0x123;
		uint8_t length = 8;
		uint8_t TxData[8];
		TxData[0] = 0x55;
		TxData[1] = 0x55;
		TxData[2] = 0x55;
		TxData[3] = 0x55;
		TxData[4] = 0x55;
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;
		return try_send(can_id, length, TxData);
	}
	

	return USAGE;
}

COMMAND_ENTRY(
	"can",
	"can send",
	"can read",
	do_can
)
