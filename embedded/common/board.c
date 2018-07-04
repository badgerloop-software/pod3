#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "rcc.h"
#include "pin_alias.h"
#include "console.h"
#include <string.h>

/* STM32L432KC */
#include "stm32l4xx_hal.h"

/* Nucleo 32 I/O */
	FILL_GPIO(LED,		GPIOB, 3, OUTPUT, LOW_SPEED, NONE, true, LED)
	FILL_AFIO(UART_TX,	GPIOA, 2, ALT, 7, MEDIUM_SPEED, NONE, true, OTHER)
	FILL_AFIO(UART_RX,	GPIOA, 15, ALT, 3, MEDIUM_SPEED, NONE, true, OTHER)

	//I2C
	FILL_AFIO(I2C1_SCL,	GPIOA, 9, ALT, 4, LOW_SPEED, NONE, true, I2C)
	FILL_AFIO(I2C1_SDA,	GPIOA, 10, ALT, 4, LOW_SPEED, NONE, true, I2C)

	//CAN
	FILL_AFIO(CAN1_TX, GPIOA, 12, ALT, 9, LOW_SPEED, NONE, true, I2C)
	FILL_AFIO(CAN1_RX, GPIOA, 11, ALT, 9, LOW_SPEED, NONE, true, I2C)

/* COnfigures CAN using the HAL driver */
int CAN_Config(CAN_HandleTypeDef *hcan, char* board){
	
	CAN_FilterTypeDef sFilterConfig0;

	/* CAN */
	HAL_Init();

    /* Clock Enables */
	__HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

	/* General CAN Init */
	hcan->Instance = CAN1;
	hcan->Init.TimeTriggeredMode = DISABLE;
	hcan->Init.AutoBusOff = DISABLE;
	hcan->Init.AutoWakeUp = ENABLE;
	hcan->Init.AutoRetransmission = ENABLE;
	hcan->Init.ReceiveFifoLocked = DISABLE;
	hcan->Init.TransmitFifoPriority = DISABLE;
	hcan->Init.Mode = CAN_MODE_NORMAL;

	/* CAN Bit Timing Register Init */
	hcan->Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan->Init.TimeSeg1 = CAN_BS1_4TQ;
	hcan->Init.TimeSeg2 = CAN_BS2_3TQ;
	hcan->Init.Prescaler = 12;

    /* Prescaler Calc 
	 * Figure 452 Page 1369 of the Reference manual
	 *
	 * Baudrate = 1/Nominal_Bit_Time
	 * Baudrate = 250kbs Tpclk = 1/24000kHz
	 * (Decided Nominal_Bit_Time = 8 Time quanta)
	 * Solve for length of one time quanta: 
	 * Tq = (Prescaler + 1) * Tpclk
	 *
	 */

	/* CAN Filter Config */
	sFilterConfig0.FilterBank = 0;
	sFilterConfig0.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig0.FilterScale = CAN_FILTERSCALE_32BIT;
   	sFilterConfig0.FilterFIFOAssignment = CAN_RX_FIFO0;
  	sFilterConfig0.SlaveStartFilterBank = 14;
	
	/* Board Specific (Filter) Initialization */
	/* For CAN ID filtering, look at: https://community.st.com/thread/28968 */
	// TODO: Come up with filter values for boards
    if( strcmp(board, "nav") == 0){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
  		sFilterConfig0.FilterMaskIdHigh = 0x0000;
   		sFilterConfig0.FilterMaskIdLow = 0x0000;
	}
	else if( strcmp(board, "dash") == 0){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
  		sFilterConfig0.FilterMaskIdHigh = 0x0000;
   		sFilterConfig0.FilterMaskIdLow = 0x0000;
	}
	else if( strcmp(board, "pv") == 0){
		sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
  		sFilterConfig0.FilterMaskIdHigh = 0x0000;
   		sFilterConfig0.FilterMaskIdLow = 0x0000;
	}
	else if( strcmp(board, "dev") == 0){
		sFilterConfig0.FilterIdHigh = 0x29F << 5;
		//sFilterConfig0.FilterIdHigh = 0x0000;
		sFilterConfig0.FilterIdLow = 0x0000;
		sFilterConfig0.FilterMaskIdHigh = 0x7FF << 5;
  		//sFilterConfig0.FilterMaskIdHigh = 0x0000;
   		sFilterConfig0.FilterMaskIdLow = 0x0000;
	}
	else{
		printf("Incorrect Usage. Include a Board Name.\r\n");
		return 1;
	}	
	sFilterConfig0.FilterActivation = ENABLE;
	
    /* Calling CAN Init Functions */
	if(HAL_CAN_Init(hcan) != HAL_OK){
		printf("CAN Init Error.\r\n");
		return 1;
	}
	if(HAL_CAN_ConfigFilter(hcan, &sFilterConfig0) ){
		printf("CAN Filter Error.\r\n");
		return 1;
	}
	if(HAL_CAN_Start(hcan) != HAL_OK){
		printf("CAN Start Error.\r\n");
		return 1;
	} 	
	return 0;
}


int io_init(void) {

	int ret = 0;

	/* set HCLK = SYSCLK / 2 */
	rcc_set_AHB(8);

	/* set PLL configuration and make it the system clock source: */
	/* ((16 MHz / 2) * 24) / 4 = 48 MHz SYSCLK */
	ret += rcc_setPLLs(HSI, 24, 1, 0, 1, 1);
	rcc_changeSysClockSrc(PLL);

	/* turn off unused MSI */
	rcc_setClk(MSI, false);

	ret += gpioAliasInit();

	return ret;
}

int periph_init( CAN_HandleTypeDef *hcan, char* board ) {

	int ret = 0;
	uint32_t init_regs[3] = {0, 0, 0};

	/* USB UART */
	init_regs[0] = USART_CR1_RXNEIE;
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);
	ret += usart_config(USART1, SYSCLK, init_regs, 115200, true);

	process_input("i2c init");

    CAN_Config( hcan, board);

	return ret;
}

inline void blink_handler(unsigned int blink_int) {

	static unsigned int curr = 0, prev = 0;

	curr = ticks / blink_int;
	if (curr != prev) 
		(curr % 2) ? 
			gpio_writePin(GPIOB, 3, 1) : gpio_writePin(GPIOB, 3, 0);
		
	prev = curr;
}

void fault(void) {

	printf("Entered %s\r\n", __func__);

	while (1) {
		blink_handler(750);
	}
}

inline void set_led(bool state) {
	(state) ? gpio_writePin(GPIOB, 3, 1) : gpio_writePin(GPIOB, 3, 0);
}
