#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"

#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_ll_adc.h"

ADC_HandleTypeDef AdcHandle;

#define BLINK_INTERVAL	100

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

    int ADCValue;
    char* str = "";

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init() || dev_init())
		fault();

	rx = get_rx(USB_UART);

	post("Developmental");
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);

        HAL_ADC_Start(&AdcHandle);
        if (HAL_ADC_PollForConversion(&AdcHandle, 1000000) == HAL_OK)
        {
            ADCValue = HAL_ADC_GetValue(&AdcHandle);
           	sprintf(str, "%d", ADCValue);
            printf("ADC: %s\r\n", str);
        }
        HAL_ADC_Stop(&AdcHandle);

   
    }

	return 0;
}
