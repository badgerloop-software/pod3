#include <stdio.h>
#include "commands.h"

command_status do_can(int argc, char *argv[]) {
	printf("%s: TODO (%d args given)\r\n", argv[0], argc);
	return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"can",
	"can TODO",
	"TODO.",
	do_can
)
