#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "memmap.h"

const mmap_t CODE[] = {
	MIN_MMAP_ENTRY("FLASH", 0x0, 0x40000)
};

const mmap_t TOP_LEVEL[] = {
	MMAP_ENTRY("CODE", 0x0, 0x20000000, CODE)
	MIN_MMAP_ENTRY("SRAM1", 0x20000000, 0xC000)
	MIN_MMAP_ENTRY("SRAM2", 0x2000C000, 0x4000)
	MIN_MMAP_ENTRY("PERIPH", 0x40000000, 0x10060C00)
	MIN_MMAP_ENTRY("QSPIFL", 0x90000000, 0x10000000)
	MIN_MMAP_ENTRY("QSPIRG", 0xA0001000, 0x400)
	MIN_MMAP_ENTRY("M4 CORE", 0xE0000000, 0x20000000)
};

command_status do_memmap(int argc, char *argv[]) {

	int i, idx;
	uint32_t size;
	const mmap_t *curr;

	curr = TOP_LEVEL;
	size = sizeof(TOP_LEVEL) / sizeof(mmap_t);

	/* find desired entry */
	for (i = 1; i < argc; i++) {

		/* don't try to index it if there aren't children */
		if (!size || !curr) {
			printf("no children at this depth (%d)\r\n", i - 1);
			return FAIL;
		}

		if ((idx = find_entry(argv[i], curr, size)) < 0) {
			printf("couldn't find '%s' at this depth (%d)\r\n", argv[i], i - 1);
			return FAIL;
		}

		size = curr->num_children;
		curr = curr->children;
	}

	print_all(curr, size);

	return SUCCESS;
}

COMMAND_ENTRY(
	"memmap", "memmap [section...]",
	"Print information about the memory layout of this device.", do_memmap
)

void print_mmap_t(const mmap_t *entry) {
	printf(
		"%-10s:\t0x%08lx - 0x%08lx (%lu%c)\r\n",
		entry->name, entry->addr, entry->addr + entry->size - 1,
		entry->size / 1024, 'k'
	);
}

void print_all(const mmap_t start[], int count) {
	int i;
	for (i = 0; i < count; i++) print_mmap_t(&start[i]);
}

int find_entry(const char *name, const mmap_t start[], int count) {
	int i;
	for (i = 0; i < count; i++)
		if (!strcmp(start[i].name, name)) return i;
	return -1;
}
