#include <stdio.h>
#include "commands.h"
#include "badgerloop.h"
#include "string.h"
#include "gpio.h"

command_status do_bms(int argc, char *argv[]) {

	if (argc != 2) {
		return USAGE;
	} else {
	    if( !strcmp( argv[1], "set")){
            	bms_software_reset_set( true );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "clear") ){
            	bms_software_reset_set( false );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "read") ){
            	printf("PB Reset Pin Value: %d\r\n", gpio_readPin(GPIOB, 4)  );
            	return CMD_SUCCESS;
        }
        return USAGE;
    }
	
    return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"bms",
	"bms <set/clear>",
	"Sets or clears the PB Reset pin.",
	do_bms
)

command_status do_mcu(int argc, char *argv[]) {

	if (argc != 2) {
		return USAGE;
	} else {
	    if( !strcmp( argv[1], "set")){
            	mcu_high_voltage_set( true );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "clear") ){
            	mcu_high_voltage_set( false );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "read") ){
            	printf("MCU HV EN Value: %d\r\n", gpio_readPin(GPIOA, 6)  );
            	return CMD_SUCCESS;
        }
        return USAGE;
    }
	
    return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"mcu_hv",
	"mcu_hv <set/clear>",
	"Sets or clears the MCU HV EN pin.",
	do_mcu
)

command_status do_pv_sol2(int argc, char *argv[]) {

	if (argc != 2) {
		return USAGE;
	} else {
	    if( !strcmp( argv[1], "set")){
            	pv_solenoid2_set( true );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "clear") ){
                pv_solenoid2_set( false );
            	return CMD_SUCCESS;
            } else if( !strcmp( argv[1], "read") ){
            	printf("PV Solenoid 2 Value: %d\r\n", gpio_readPin(GPIOA, 1)  );
            	return CMD_SUCCESS;
        }
        return USAGE;
    }
	
    return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"pv_sol2",
	"pv_sol2 <set/clear/read>",
	"Sets or clears the PV Sol2 pin.",
	do_pv_sol2
)
