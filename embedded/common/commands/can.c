#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "can.h"
#include <limits.h>
#include "badgerloop.h"
#include <stm32l4xx_hal.h>
#include <unistd.h>

/* Globals */
extern uint8_t message_num;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
extern uint8_t message_num; //defined in can.h

int can_read(CAN_HandleTypeDef *hcan ){
	int i;
	if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0)){
		printf("CAN Message Received.\r\n");
	    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
        
        /* Printing out received data */
		printf("Received CAN ID: #%lx\r\n", RxHeader.StdId );
		for(i = 0; i < 8; i++){
			if( RxData[i] != 0){
				printf("CAN Message Data #%d: %x\r\n", i, RxData[i]);
			}
		}
	}

	return 0;
}

int can_send(uint32_t can_id, size_t length, uint8_t *TxData, CAN_HandleTypeDef *hcan){

#if DEBUG_IO
	printf("\r\nCAN SEND ID: %lx, length: %d\r\n", can_id, length);
#endif

	TxHeader.StdId = can_id;
	TxHeader.IDE = 0;
	TxHeader.RTR = 0;
	if( length % 2 == 1){
		TxHeader.DLC = (length/2) + 1;
	}
	else{
		TxHeader.DLC = length/2;
	}
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan)){
		printf("SENDING MESSAGE\r\n");
		uint32_t TxMailbox = 0;
		
		if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK) return 0;
		
		
	}
	return 1;
}

//Temporary function for updating message_num (heartbeat related) from the command line
int can_update( uint8_t message_number ){
	message_num = message_number;
	return 0;
}

uint8_t * can_send_obd2(uint16_t can_id, size_t message_length, uint8_t mode, uint16_t pid, CAN_HandleTypeDef *hcan){
	
	int i = 0;
	//OBD2 Message format: CAN_ID, CAN Message length, OBDII message length, Mode, PID, Data

	TxHeader.StdId = can_id;
	TxHeader.IDE = 0;
	TxHeader.RTR = 0;
	TxHeader.DLC = (uint8_t) 8;

	uint8_t TxData[8];
	TxData[0] = message_length;
	TxData[1] = mode;
	TxData[2] = (pid & 0xff);
	TxData[3] = (pid & 0xff00) >> 8;
	TxData[4] = 0x55;
	TxData[5] = 0x55;
	TxData[6] = 0x55;
	TxData[7] = 0x55;
	printf("In OBD2 Send \r\n");	
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan)){
		printf("SENDING MESSAGE\r\n");
		uint32_t TxMailbox = 0;
		
		if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
			printf("ADD TX MESSAGE ERROR\r\n");

		} else {
			printf("SEND OK\r\n");
		}

		printf("TxMailbox after sending %lu\r\n",TxMailbox);
	}

	
	if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0)){
		printf("CAN MESSAGE IN FIFO\r\n");
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
		
		for(i = 0; i < 8; i++){
			if( RxData[i] != 0){
				printf("Here is our message %x\r\n", RxData[i]);
			}
		}
	}

	return RxData;
}


int can_heartbeat(uint8_t *message_number, CAN_HandleTypeDef *hcan){
	int i = 0;
	TxHeader.StdId = 0xC0; //Always to same CAN ID
	TxHeader.IDE = 0; //Standard ID length
	TxHeader.RTR = 0; //Always data frame
	TxHeader.DLC = (uint8_t) 8; //Always 8
	uint8_t number = *message_number;
	uint8_t TxData[8];

	uint32_t TxMailbox = 0;

	if( number == 2){ //Command 5 nm forwardi
		TxData[0] = 0x32;
		TxData[1] = 0x00;
		TxData[2] = 0x00;
		TxData[3] = 0x00;
		TxData[4] = 0x00;
		TxData[5] = 0x01;
		TxData[6] = 0x00;
		TxData[7] = 0x00;
	}
	else if( number == 3){ //Command 8 nm forward
		TxData[0] = 0x50;
		TxData[1] = 0x00;
		TxData[2] = 0x00;
		TxData[3] = 0x00;
		TxData[4] = 0x00;
		TxData[5] = 0x01;
		TxData[6] = 0x00;
		TxData[7] = 0x00;
	}
	else if( number == 4){ //Discharge capacitors
		TxData[0] = 0x00;
		TxData[1] = 0x00;
		TxData[2] = 0x00;
		TxData[3] = 0x00;
		TxData[4] = 0x01;
		TxData[5] = 0x02;
		TxData[6] = 0x00;
		TxData[7] = 0x00;
	}
	else if( number == 5 ){ //Command 16 nm forward
		TxData[0] = 0xA0;
		TxData[1] = 0x00;
		TxData[2] = 0x00;
		TxData[3] = 0x00;
		TxData[4] = 0x00;
		TxData[5] = 0x01;
		TxData[6] = 0x00;
		TxData[7] = 0x00;
	}
	else{ //In all other cases, TxData will be set to 0
		for(i = 0; i < 8; i++){
			TxData[i] = 0;
		}
	}

	if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
		return 1; //Returns 1 on error
	} else {
		return 0; //Returns 0 on success
	}
}

int can_clearFaults( CAN_HandleTypeDef *hcan ){
	
	TxHeader.StdId = 0xC1; //Always to same CAN ID
	TxHeader.IDE = 0; //Standard ID length
	TxHeader.RTR = 0; //Always data frame
	TxHeader.DLC = (uint8_t) 8; //Always 8
	uint8_t TxData[8];

	uint32_t TxMailbox = 0;
		
	TxData[0] = 0x14;
	TxData[1] = 0x00;
	TxData[2] = 0x01;
	TxData[3] = 0x00;
	TxData[4] = 0x00;
	TxData[5] = 0x00;
	TxData[6] = 0x00;
	TxData[7] = 0x00;

	if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
		return 1; //Returns 1 on error
	} else {
		return 0; //Returns 0 on success
	}
}


int intermodule_can_message(SENDING_MODULE sending_module, RECEIVING_MODULE receiving_module, int message_num, MESSAGE_TYPE message_type, uint8_t *TxData, CAN_HandleTypeDef *hcan){
	uint16_t can_id = (sending_module << 8) + (receiving_module << 4) + message_num;
	printf("CAN_ID %x\r\n", can_id);
		
	TxData[0] = message_type;
	TxData[1] = TxData[0];
	TxData[2] = TxData[1];
	TxData[3] = TxData[2];
	TxData[4] = TxData[3];
	TxData[5] = TxData[4];
	TxData[6] = TxData[5];
	TxData[7] = TxData[6];
	uint8_t length = 8;

	return can_send(can_id, length, TxData, hcan);
}

command_status do_can(int argc, char *argv[]) {

	printf("%s:  (%d args given)\r\n", argv[0], argc);
	long long converted = 0;	
	uint8_t data[8];
	char* str;
	int i;
	
	//Usage: can read
	if(!strcmp("read", argv[1])){	

		printf("\r\ncalling can_read()\r\n");
		can_read(&hcan);

	//Usage: can send <can ID> <data>
	} else if(!strcmp("send", argv[1])){
		
		
		uint32_t can_id = strtol(argv[2], &str, 16);
			if(can_id > 0x7ff){
				printf("Invalid CAN ID.\r\n");
				return ERROR;
			}
			printf( "CAN ID: %lx\r\n", can_id);
			if(converted == LONG_MAX || converted == LONG_MIN ){
				printf("strtoll() error.\r\n" );
			}
			else if( str == argv[2] ){
				printf("No digits were found.\r\n");
			}
			else if( *str != '\0'){
				printf("Extra string passed in.\r\n");
			}


		size_t length = strlen(argv[3]);

		converted = strtoll(argv[3], &str, 16);
		if(converted == LONG_MAX || converted == LONG_MIN ){
			printf("strtoll() error.\r\n" );
		}
		else if( str == argv[3] ){
			printf("No digits were found.\r\n");
		}
		else if( *str != '\0'){
			printf("Extra string passed in.\r\n");
		}
		
		
		data[7] = (converted & 0xff00000000000000) >> 56;
		data[6] = (converted & 0x00ff000000000000) >> 48;
		data[5] = (converted & 0x0000ff0000000000) >> 40;
		data[4] = (converted & 0x000000ff00000000) >> 32;
		data[3] = (converted & 0x00000000ff000000) >> 24;
		data[2] = (converted & 0x0000000000ff0000) >> 16;
		data[1] = (converted & 0x000000000000ff00) >> 8;
		data[0] = (converted & 0x00000000000000ff);

		printf("can_id: %lx, length: %d\r\n", can_id, length);
		for(i = 0; i < 8; i++){
			printf("Data #%d: %x\r\n", i,data[i]);
		}
		if( length > 0 && length <= 16){
			can_send(can_id, length, data, &hcan);
		}
		else{
			printf("Data must be <= 16 hex values and > 0.");
			return ERROR;
		}
	
	} else if(!strcmp("update", argv[1])){

		can_update(atoi(argv[2]));
		printf( "Message Number: %d\r\n", message_num);
	}	
	return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"can",
	"can send",
	"can read",
	do_can
)
