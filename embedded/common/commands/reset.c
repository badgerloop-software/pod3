#include <stdio.h>
#include "commands.h"

command_status do_reset(int argc, char *argv[]) {
	printf("%s: TODO (%d args given)\r\n", argv[0], argc);
	return SUCCESS;
}

COMMAND_ENTRY(
	"reset",
	"reset",
	"Perform a software reset.",
	do_reset
)
