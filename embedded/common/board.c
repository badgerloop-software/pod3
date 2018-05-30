#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "rcc.h"
#include "pin_alias.h"

/* STM32L432KC */
// TODO
#include "stm32l4xx_hal.h"
uint32_t HAL_GetTick(void) {return ticks;}
void CAN_Config(CAN_HandleTypeDef hcan);

/* Nucleo 32 I/O */
	FILL_GPIO(LED,		GPIOB, 3, OUTPUT, LOW_SPEED, NONE, true, LED)
	FILL_AFIO(UART_TX,	GPIOA, 2, ALT, 7, MEDIUM_SPEED, NONE, true, OTHER)
	FILL_AFIO(UART_RX,	GPIOA, 15, ALT, 3, MEDIUM_SPEED, NONE, true, OTHER)

	//I2C
	FILL_AFIO(I2C1_SCL,	GPIOA, 9, ALT, 4, LOW_SPEED, NONE, true, I2C)
	FILL_AFIO(I2C1_SDA,	GPIOA, 10, ALT, 4, LOW_SPEED, NONE, true, I2C)

	//CAN
	FILL_AFIO(CAN1_TX, GPIOA, 12, ALT, 9, LOW_SPEED, NONE, true, OTHER)
	FILL_AFIO(CAN1_RX, GPIOA, 11, ALT, 9, LOW_SPEED, NONE, true, OTHER)


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

int periph_init(CAN_HandleTypeDef hcan) {

	int ret = 0;
	uint32_t init_regs[3] = {0, 0, 0};
	printf("call CAN_Config()");
	/* CAN Configuration */
	CAN_Config(hcan);

	/* USB UART */
	init_regs[0] = USART_CR1_RXNEIE;
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);


	return ret;
}



void CAN_Config(CAN_HandleTypeDef hcan){
	/* CAN */
	HAL_Init();
	printf("HAL_INIT OK\r\n");

	__HAL_RCC_CAN1_CLK_ENABLE();
        printf("HALL_RCC_CLK_ENABLE() OK\r\n");
	__HAL_RCC_GPIOA_CLK_ENABLE();
	printf("HAL_GPIOA_CLK_ENABLE() OK\r\n");
        
	
	/* Interrupt based functions
	 * Uncomment to (partially) enable interrupts
	HAL_NVIC_SetPriority(CAN1_TX_IRQn,0,0);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn,0,0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn,0,0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn,0,0);

        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
        HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
	* Uncomment to (partially) enable interrupts
	* Interrupt handlers
	*/
	





	
	//TODO check instance
	/* CAN FILTER */
	
	hcan.Instance = CAN1;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = ENABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_3TQ;
	
	/* Prescaler Calc 
	 * Figure 452 Page 1369 of the Reference manual
	 *
	 * Baudrate = 1/Nominal_Bit_Time
	 * Baudrate = 250kbs Tpclk = 24000kHz
	 * (Decided Nominal_Bit_Time = 8 Time quanta)
	 * Solve for length of one time quanta: 
	 * Tq = (Prescaler + 1) * Tpclk
	 *
	 */
	hcan.Init.Prescaler = 11;
	

	/* CAN Filter */
	printf("CAN FILTER INIT");	
	//TODO Check
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
   	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
   	sFilterConfig.FilterIdHigh = 0xffff;
   	sFilterConfig.FilterIdLow = 0x0000;
   	sFilterConfig.FilterMaskIdHigh = 0xffff;
   	sFilterConfig.FilterMaskIdLow = 0x0000;
   	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  	sFilterConfig.FilterActivation = ENABLE;
   	sFilterConfig.SlaveStartFilterBank = 14;

	if(HAL_CAN_Init(&hcan) != HAL_OK){
		printf("CAN INIT ERROR \r\n");
	} else {
		printf("CAN Init OK\r\n");
	}

	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig)){
		printf("CAN Filter Errror\r\n");
	} else {
		printf("CAN Filter OK\r\n");
	}

	if(HAL_CAN_Start(&hcan) != HAL_OK){
		printf("CAN Start Error\r\n");
	} else {
		printf("CAN Started OK\r\n");
	} 		
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
