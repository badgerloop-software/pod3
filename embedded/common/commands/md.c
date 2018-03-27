#include <stdio.h>
#include "commands.h"

command_status do_md(int argc, char *argv[]) {
	UNUSED(argc); UNUSED(argv);
	printf("TODO\r\n");
	return SUCCESS;
}

COMMAND_ENTRY(
	"md",
	"md TODO",
	"md desc TODO.",
	do_md
)
