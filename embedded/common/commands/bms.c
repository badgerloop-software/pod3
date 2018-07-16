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
