#include <stdio.h>
#include "commands.h"

command_status do_i2c(int argc, char *argv[]) {
	printf("%s: TODO (%d args given)\r\n", argv[0], argc);
	return SUCCESS;
}

COMMAND_ENTRY(
	"i2c",
	"i2c TODO",
	"TODO.",
	do_i2c
)
