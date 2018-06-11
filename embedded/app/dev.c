#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "stm32l4xx_hal.h"
#include "can.h"

/* CAN Globals */
uint8_t ubKeyNumber = 0x0;
extern CAN_HandleTypeDef     hcan;

inline void printPrompt(void) {
	fputs("[dev-build] $ ", stdout);
	fflush(stdout);
}

int dev_init(void) {
	/* dev specific initializations */
	return 0;
}

int main(void) {
	PC_Buffer *rx; /* Serial */
//	int ticks; /* Used for SysTick operation */

	/* CAN Variables */
//	CAN_TxHeaderTypeDef   TxHeader;
	CAN_RxHeaderTypeDef   RxHeader;
//	uint8_t               TxData[8];
	uint8_t               RxData[8];
//	uint32_t   	      TxMailbox;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(hcan) || dev_init())
		fault();

	/* Initializing TX Header and Data for CAN messages */
/*	TxHeader.StdId = 0x7e3;
	TxHeader.IDE = 0;
	TxHeader.RTR = 0;
	TxHeader.DLC = 8;	

	TxData[0] = 0x01;
	TxData[1] = 0x3e;
	TxData[2] = 0;
	TxData[3] = 0;
	TxData[4] = 0;
	TxData[5] = 0;
	TxData[6] = 0;
	TxData[7] = 0;
	
	TxMailbox = 0;
*/
	rx = get_rx(USB_UART); /* Serial Init */
	/* CAN Init */
	while( CAN_Config(&hcan, "dev") != 0){
		printf("CAN Config Error.\r\n");
	}	
	
	printPrompt();
	printf("Init OK\r\n");
	fflush(stdout);
	while (1) {
		check_input(rx); /* Check for Serial Input */
		
		/* Check for incoming CAN messages */
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)){
			printf("FILL LEVEL: %lu\r\n", HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0));
			HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
			/* Printing out received data */
			for( int i = 0; i < 8; i++){
				//if(RxData[i] != 0){
					printf("CAN Message Data #%d %x\r\n", i, RxData[i]);
				//}
			}
		
		}

	
		/*SysTick configured to send messages every 500 ms */
	
	/*
		ticks = HAL_GetTick();
		static unsigned int curr = 0, prev = 0;
		curr = ticks / 500;
		if (curr != prev){
			prev = curr;
			//printf( "Sending a message\r\n");
			if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox)!= HAL_OK){
				printf("HAL_CAN_AddTxMessage Error.\r\n");

			} else {
				//printf("Send OK.\r\n");
			}
			fflush(stdout);
		}
		*/
	
	}
}

