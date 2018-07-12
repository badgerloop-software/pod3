#include "commands.h"
#include <stdio.h>
#include "bms.h"

command_status do_bms(int argc, char *argv[]) {
	if(argc < 1) return USAGE;
	printf("argv: %s\r\n", argv[0]);

	printf("\r\n");
	printf("BMS INFO\r\n");
	printf("Current: %d A\tVoltage: %d A\tSOC: %d\r\n", bms->packCurrent,
			bms->packVoltage, bms->Soc);
	return CMD_SUCCESS;

}

COMMAND_ENTRY(
		"bms",
		"bms <disp>",
		"Show BMS Options",
		do_bms)
