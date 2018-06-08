#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"

#define BLINK_INTERVAL	100

/* CAN Globals */
uint8_t ubKeyNumber = 0x0;
CAN_HandleTypeDef     hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

inline void printPrompt(void) {
	fputs("[dev-build] $ ", stdout);
	fflush(stdout);
}

int dev_init(void) {
	/* dev specific initializations */
	return 0;
}

int main(void) {
	PC_Buffer *rx;
	
	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(hcan) || dev_init())
		fault();

	rx = get_rx(USB_UART);
	CAN_Config(&hcan, "dev");
	
	fflush(stdout);
	printPrompt();
	printf("Init OK\r\n");
	fflush(stdout);
	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
		
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)){
			printf("CAN MESSAGE IN FIFO\r\n");
			HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
			for( int i = 0; i < 8; i++){
				if(RxData[i] != 0){
					printf("Here is our message #%d %x\r\n", i, RxData[i]);
				}
			}
		
		}
	}
}

