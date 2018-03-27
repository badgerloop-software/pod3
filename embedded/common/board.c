#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "rcc.h"
#include "pin_alias.h"

/* STM32L432KC */
// TODO

/* Nucleo 32 I/O */
FILL_GPIO(LED,		GPIOB, 3, OUTPUT, LOW_SPEED, NONE, true, LED)
FILL_AFIO(UART_TX,	GPIOA, 2, ALT, 7, MEDIUM_SPEED, NONE, true, OTHER)
FILL_AFIO(UART_RX,	GPIOA, 15, ALT, 3, MEDIUM_SPEED, NONE, true, OTHER)

//LEDs
FILL_GPIO(LED_A,	GPIOA, 7, OUTPUT, LOW_SPEED, NONE, true, LED);
FILL_GPIO(LED_B,	GPIOA, 6, OUTPUT, LOW_SPEED, NONE, true, LED);
FILL_GPIO(LED_C,	GPIOA, 5, OUTPUT, LOW_SPEED, NONE, true, LED);

//I2C
FILL_AFIO(I2C1_SCL, GPIOA, 9, ALT, 4, LOW_SPEED, NONE, true, I2C );	
FILL_AFIO(I2C1_SDA, GPIOA, 10, ALT, 4, LOW_SPEED, NONE, true, I2C );

//CAN
FILL_AFIO(CAN1_TX, GPIOA, 11, ALT, 12, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(CAN1_RX, GPIOA, 12, ALT, 11, LOW_SPEED, NONE, true, OTHER );
	
//IMU
FILL_GPIO(IMU_nRST,	GPIOB, 1, OUTPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_DRDY,	GPIOC, 14, OUTPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_SYNC_IN,	GPIOC, 15, OUTPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_SYNC_IN,	GPIOA, 8, OUTPUT, LOW_SPEED, NONE, true, SENSOR);

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );

//Switches
FILL_GPIO(SW_A,	GPIOA, 3, OUTPUT, LOW_SPEED, NONE, true, OTHER); //VALVE maybe?
FILL_GPIO(SW_B,	GPIOA, 1, OUTPUT, LOW_SPEED, NONE, true, OTHER); //VALVE maybe?

	
/* CAN */
// TODO

/* I2C */
// TODO

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
