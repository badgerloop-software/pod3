#include <string.h>
#include <stdio.h>
#include "can.h"
#include <stm32l4xx_hal.h>
#include <unistd.h>
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];


char* can_read(CAN_HandleTypeDef hcan ){
	printf("\r\nIn CAN READ\\");
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






	return "TEST";
}


void can_send(uint16_t can_id, uint64_t length, uint64_t data, CAN_HandleTypeDef hcan){
	printf("\r\nCAN SEND ID: %d length: %llu data: %llu\r\n", can_id, length, data);


	TxHeader.StdId = 0;
	TxHeader.IDE = 0;
	TxHeader.RTR = 0;
	TxHeader.DLC = 8;
	
	if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)){
		printf("SENDING MESSAGE");
		uint8_t TxData[8];
		TxData[0] = 0x55;
		TxData[1] = 0x55;
		TxData[2] = 0x55;
		TxData[3] = 0x55;
		TxData[4] = 0x55;
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;
		uint32_t TxMailbox = 0;
		
		if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
			printf("ADD TX MESSAGE ERROR");

		} else {
			printf("SEND OK");
		}

		printf("TxMailbox after sending %lu",TxMailbox);
	}
	printf("state after message send: %d", hcan.State);	
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
}
