#include <stdio.h>
#include "commands.h"
#include "stm32l432xx.h"
#include "core_cm4.h"

command_status do_reset(int argc, char *argv[]) {
	UNUSED(argc); UNUSED(argv);
	NVIC_SystemReset();
	return CMD_SUCCESS;
}

COMMAND_ENTRY("reset", "reset", "Perform a software reset.", do_reset)
