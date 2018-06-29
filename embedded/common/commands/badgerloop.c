
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "can.h"
#include <limits.h>
#include "badgerloop.h"
extern uint8_t message_num;

command_status do_badgerloop(int argc, char *argv[]) {
	printf("%s:  (%d args given)\r\n", argv[0], argc);
	
//	printf("Calling Telemetry Send Message");
//	telemetry_setup();	


	return SUCCESS;
}

COMMAND_ENTRY(
	"badgerloop",
	"badgerloop send",
	"badgerloop read",
	do_badgerloop
)
