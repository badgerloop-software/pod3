#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "adc.h"

command_status do_adc_init(void){
	return (adc_init() == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status adc_try_read( uint8_t channel ){
	
    printf("ADC %lu", adc_read( channel ));
	return CMD_SUCCESS;
}

command_status do_adc(int argc, char *argv[]){
	if (argc == 1) return USAGE;

	if (!strcmp("init", argv[1]))
		return do_adc_init();


	if (!strcmp("read", argv[1])){
		return adc_try_read( (uint8_t)(atoi(argv[2])) );
	}
	return USAGE;
}

COMMAND_ENTRY(
	"adc",
	"adc read <channel>",
	"adc init",
	do_adc
)
