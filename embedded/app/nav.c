#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"

#define BLINK_INTERVAL	250

/* Nucleo 32 I/O */
	//LEDs
	FILL_GPIO(LED_A,	GPIOA, 7, OUTPUT, LOW_SPEED, NONE, true, LED);
	FILL_GPIO(LED_B,	GPIOA, 6, OUTPUT, LOW_SPEED, NONE, true, LED);
	FILL_GPIO(LED_C,	GPIOA, 5, OUTPUT, LOW_SPEED, NONE, true, LED);
	
	//IMU
	FILL_GPIO(IMU_nRST,	GPIOB, 1, OUTPUT, LOW_SPEED, NONE, true, SENSOR);
	FILL_GPIO(IMU_DRDY,	GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR);
	FILL_GPIO(IMU_SYNC_OUT,	GPIOC, 15, INPUT, LOW_SPEED, NONE, true, SENSOR);
	FILL_GPIO(IMU_SYNC_IN,	GPIOA, 8, OUTPUT, LOW_SPEED, NONE, true, SENSOR);

	//SPI
	FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER );
	FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER );
	FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );
	FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );

	//Switches
	FILL_GPIO(SW_A,	GPIOA, 3, OUTPUT, LOW_SPEED, NONE, true, OTHER); //VALVE maybe?
	FILL_GPIO(SW_B,	GPIOA, 1, OUTPUT, LOW_SPEED, NONE, true, OTHER); //VALVE maybe?

	//UART
	FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER );
	FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER );


inline void printPrompt(void) {
	fputs("[nav-build] $ ", stdout);
	fflush(stdout);
}

int nav_init(void) {

	/* nav specific initializations */

	return 0;
}

int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init() || nav_init())
		fault();

	rx = get_rx(USB_UART);

	post("Navigation");
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
