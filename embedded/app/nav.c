#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"

#define BLINK_INTERVAL	250

/* Nucleo 32 I/O */

//Limit Switches
//Lim1: Door engaged
//Lim2: Primary braking engaged
//Lim3: Secondary Braking engaged
FILL_GPIO(LIM1, GPIOA, 3, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(LIM2, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(LIM3,	GPIOA, 7, INPUT, LOW_SPEED, NONE, true, SENSOR);

//Retros
// Retros 1 2 and 3
FILL_GPIO(RETRO1, GPIOA, 0, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(RETRO2, GPIOA, 1, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(RETRO3, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR);

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
