#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "commands.h"

command_status do_md(int argc, char *argv[]) {

	uint32_t *addr;

	if (argc < 2) return USAGE;

	errno = 0;

	addr = (uint32_t *) strtoul(argv[1], NULL, 16);

	if (errno) {
		printf("couldn't convert '%s' to a 32-bit address\r\n", argv[1]);
		return FAIL;
	}

	printf("[@0x%08lx] 0x%lx\r\n", (uint32_t) addr, *addr);

	return SUCCESS;
}

COMMAND_ENTRY(
	"md", "md hex_addr",
	"Display the contents of a certain memory address.", do_md
)
