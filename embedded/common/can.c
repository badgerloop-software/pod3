#include <string.h>
#include <stdio.h>
#include "can.h"
#include <stm32l4xx_hal.h>
#include <unistd.h>
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];


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
	 	/*
		TxData[0] = (uint8_t) 0x55;
		TxData[1] = (uint8_t) 0x55;
		TxData[2] = (uint8_t) 0x55;
		TxData[3] = (uint8_t) 0x55;
		TxData[4] = (uint8_t) 0x55;
		TxData[5] = (uint8_t) 0x55;
		TxData[6] = (uint8_t) 0x55;
		TxData[7] = (uint8_t) 0x55;
		*/
		uint32_t TxMailbox = 0;
		
		if(HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
			printf("ADD TX MESSAGE ERROR\r\n");

		} else {
		//	printf("SEND OK\r\n");
		}
	}

	/* LOOPBACK MODE STUFF
	for(int i = 0; i < 1000; i++){
	
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)){
			printf("CAN MESSAGE IN FIFO\r\n");
			HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
			printf("Here is our message #0 %x\r\n", RxData[0]);
			printf("Here is our message #1 %x\r\n", RxData[1]);
			printf("Here is our message #2 %x\r\n", RxData[2]);
			printf("Here is our message #3 %x\r\n", RxData[3]);
			printf("Here is our message #4 %x\r\n", RxData[4]);
			printf("Here is our message #5 %x\r\n", RxData[5]);
			printf("Here is our message #6 %x\r\n", RxData[6]);
			printf("Here is our message #7 %x\r\n", RxData[7]);
		break;
		}
	}
	*/
}



uint8_t * can_send_obd2(uint16_t can_id, size_t message_length, uint8_t mode, uint16_t pid, CAN_HandleTypeDef *hcan){
	
	TxHeader.StdId = can_id;
	TxHeader.IDE = 0;
	TxHeader.RTR = 0;
	TxHeader.DLC = (uint8_t) 8;
	// Required message format:
	// Number of additional bytes:
	// mode:
	// pid code:
	
	uint8_t TxData[8];
	TxData[0] = message_length;
	TxData[1] = mode;
	TxData[2] = pid;
	TxData[3] = 0x55;
	TxData[4] = 0x55;
	TxData[5] = 0x55;
	TxData[6] = 0x55;
	TxData[7] = 0x55;
	printf("In OBD2 Send \r\n");	
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan)){
		printf("SENDING MESSAGE\r\n");
	 	/*
		TxData[0] = (uint8_t) 0x55;
		TxData[1] = (uint8_t) 0x55;
		TxData[2] = (uint8_t) 0x55;
		TxData[3] = (uint8_t) 0x55;
		TxData[4] = (uint8_t) 0x55;
		TxData[5] = (uint8_t) 0x55;
		TxData[6] = (uint8_t) 0x55;
		TxData[7] = (uint8_t) 0x55;
		*/
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























