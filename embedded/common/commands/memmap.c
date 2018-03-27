#include <stdio.h>
#include "commands.h"

command_status do_memmap(int argc, char *argv[]) {
	UNUSED(argc); UNUSED(argv);
	printf("TODO\r\n");
	return SUCCESS;
}

COMMAND_ENTRY(
	"memmap",
	"memmap TODO",
	"memmap desc TODO.",
	do_memmap
)
