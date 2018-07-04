#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "stm32l4xx_hal.h"
#include "can.h"

#define CCP_MODULE 0 
#define NAV_MODULE 0
#define PV_MODULE 0
#define DEV_MODULE 1

/* CAN Globals */
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
	int ticks; /* Used for SysTick operation */

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(&hcan, "dev") || dev_init())
		fault();

	/* CAN Init */
	while( CAN_Config(&hcan, "dev") != 0){
		printf("CAN Config Error.\r\n");
	}	
	
    rx = get_rx(USB_UART); /* Serial Init */
	
	printPrompt();
	
    while (1) {
		check_input(rx); /* Check for Serial Input */
		
		/* Check for incoming CAN messages */
        if( can_read(&hcan) ){
            printf( "CAN Read Error.\r\n" );
        }

		/*SysTick configured to Heartbeat message every 250 ms */
		ticks = HAL_GetTick();
		static unsigned int curr = 0, prev = 0;
		curr = ticks / 250;
		if (curr != prev){
			prev = curr;
	    
            //Call the heartbeat handler
            can_heartbeat_handler(&hcan);
        }
	
	}
}
