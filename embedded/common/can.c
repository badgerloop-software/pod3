#include <string.h>
#include <stdio.h>
#include "can.h"
#include <stm32l4xx_hal.h>
#include <unistd.h>
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
extern uint8_t message_num; //defined in can.h

char* can_read(CAN_HandleTypeDef *hcan ){
	printf("\r\nIn CAN READ\\");
	
	if(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0)){
		printf("CAN MESSAGE IN FIFO\r\n");
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
		
		for(int i = 0; i < 8; i++){
			if( RxData[i] != 0){
				printf("Here is our message %x\r\n", RxData[i]);
			}
		}
	}

	return "TEST";
}


void can_send(uint32_t can_id, size_t length, uint8_t *TxData, CAN_HandleTypeDef *hcan){
	printf("\r\nCAN SEND ID: %lx, length: %d\r\n", can_id, length);


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
		
		if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
			printf("ADD TX MESSAGE ERROR\r\n");

		} else {
		//	printf("SEND OK\r\n");
		}
	}
}

//Temporary function for updating message_num (heartbeat related) from the command line
int can_update( uint8_t message_number ){
	message_num = message_number;
	return 0;
}



uint8_t * can_send_obd2(uint16_t can_id, size_t message_length, uint8_t mode, uint16_t pid, CAN_HandleTypeDef *hcan){
	
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
		
		for(int i = 0; i < 8; i++){
			if( RxData[i] != 0){
				printf("Here is our message %x\r\n", RxData[i]);
			}
		}
	}

	return RxData;
}


int can_heartbeat(uint8_t *message_number, CAN_HandleTypeDef *hcan){
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
	else{ //In all other cases, TxData will be set to 0
		for(int i = 0; i < 8; i++){
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
