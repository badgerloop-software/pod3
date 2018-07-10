#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "adc.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_ll_adc.h"

extern ADC_HandleTypeDef adc_handle;
extern uint16_t ADC5ConvertedValue;

command_status do_adc_init(void){
	return (adc_init() == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status adc_try_read(void){
	printf("ADC %d", adc_read());
	return CMD_SUCCESS;
}



command_status do_adc(int argc, char *argv[]){
	if (argc == 1) return USAGE;

	if (!strcmp("init", argv[1]))
		return do_adc_init();

	if (!strcmp("read", argv[1])){
		return adc_try_read();
	}
	return USAGE;
}

COMMAND_ENTRY(
	"adc",
	"adc read",
	"adc init",
	do_adc
)
