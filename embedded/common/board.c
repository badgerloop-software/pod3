#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "rcc.h"
#include "pin_alias.h"
#include "console.h"

/* STM32L432KC */
// TODO
#include "stm32l4xx_hal.h"
CAN_HandleTypeDef hcan;
uint32_t HAL_GetTick(void) {return ticks;}

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

int periph_init(void) {

	int ret = 0;
	uint32_t init_regs[3] = {0, 0, 0};

	/* USB UART */
	init_regs[0] = USART_CR1_RXNEIE;
	ret += usart_config(USB_UART, SYSCLK, init_regs, 115200, true);
	ret += usart_config(USART1, SYSCLK, init_regs, 115200, true);

	process_input("i2c init");

	/* CAN */
	if (HAL_CAN_Init(&hcan) != HAL_OK)
		printf("CAN Error \r\n");
	else printf("CAN Init OK\r\n");

	__HAL_RCC_CAN1_CLK_ENABLE();
	HAL_NVIC_SetPriority(CAN1_TX_IRQn,0,0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn,0,0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn,0,0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn,0,0);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

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
