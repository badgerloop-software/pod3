#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "uart.h"
#include "dashboard_control.h"

#define BLINK_INTERVAL	250
#define MODULE_DASH

/* Nucleo 32 I/O */
//THERM1: between batteries 1 and 2
//THERM2: between batteries 2 and 3
FILL_GPIO(THERM1,     GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(THERM2,	  GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR)

//SENSOR
//VOLT_SENSE is 12V battery voltage
//CURR_SENSE is 12V battery current draw, charging not included
FILL_GPIO(VOLT_SENSE, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(CURR_SENSE, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR)

//UART
FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER )

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )

//SD
FILL_GPIO(SD_CS,	 GPIOB, 0, OUTPUT, LOW_SPEED, NONE, true, OTHER )

inline void printPrompt(void) {
	fputs("[dash-build] $ ", stdout);
	fflush(stdout);
}

int dash_init() {

	/* dash specific initializations */
	/*UartHandle->Instance		 = USART1
	
	UartHandle->Init.BaudRate        = 115200;
	UartHandle->Init.WordLength      = UART_WORDLENGTH_8B;
	UartHandle->Init.StopBits	 = UART_STOPBITS_1;
	UartHandle->Init.Parity	 	 = UART_PARITY_NONE;
	UartHandle->Init.HwFlowCtl	 = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode		 = UART_MODE_TX_RX;
	UartHandle->Init.OverSampling	 = UART_OVERSAMPLING_16;
	
	if (HAL_UART_DeInit(UartHandle) != HAL_OK) printf("FAILURE TO CHECK DeInit\n\r");
	*/

	
	return 0;
}

int main(void) {

	PC_Buffer *rx;
	PC_Buffer *ctrl_rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init() || dash_init())
		fault();

	rx = get_rx(USB_UART);
	ctrl_rx = get_rx(USART1);

	post("Dashboard");
	printPrompt();

	while (1) {
		check_input(rx);
		check_incoming_controls(ctrl_rx);
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
