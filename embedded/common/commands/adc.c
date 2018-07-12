#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "adc.h"
#include "current_sense.h"
#include "voltage_sense.h"

command_status do_adc_init(void){
	return (adc_init() == HAL_OK) ? CMD_SUCCESS : FAIL;
}

command_status adc_try_read(){
	
    printf("Current Sensor: %f Amps\r\n", current_sense_read());
    printf("Voltage Sensor: %f Volts\r\n", voltage_sense_read());
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
	"adc read <channel>",
	"adc init",
	do_adc
)
