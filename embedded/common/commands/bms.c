#include <stdio.h>
#include "commands.h"
#include "badgerloop.h"

command_status do_bms(int argc, char *argv[]) {

	unsigned int i;
	int command_index;

	if (argc != 2) {
		return USAGE;
		for (i = 0; i < NUM_COMMANDS; i++)
			printf("%s\t- %s\r\n", commands[i].name, commands[i].help);
	} else {
	    if( !strcmp( argv[1], "set"){
            bms_software_reset_set( true );
            return CMD_SUCCESS;
        } else if( !strcmp( argv[1], "clear") ){
            bms_software_reset_set( false );
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
