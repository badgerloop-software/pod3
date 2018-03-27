#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "memmap.h"

#define EXP_SECT(sect)\
	((const uint32_t) &sect)

#define SECT_SZ(start, end)\
	(EXP_SECT(end) - EXP_SECT(start))

extern const unsigned int __stext, __etext, __Vectors_End, __COMMANDS_START,
	   __COMMANDS_END, __GPIO_ENTRIES_START, __GPIO_ENTRIES_END,
	   __data_start__, __data_end__, __bss_start__, __bss_end__;

const mmap_t IMAGE[] = {
	MIN_MMAP_END("TEXT", EXP_SECT(__stext), EXP_SECT(__etext))
	MIN_MMAP_END("VECTORS", EXP_SECT(__stext),  EXP_SECT(__Vectors_End))
	MIN_MMAP_END("COMMANDS", EXP_SECT(__COMMANDS_START),  EXP_SECT(__COMMANDS_END))
	MIN_MMAP_END("GPIO_ENTR", EXP_SECT(__GPIO_ENTRIES_START),  EXP_SECT(__GPIO_ENTRIES_END))
	MIN_MMAP_END("DATA",  EXP_SECT(__data_start__),  EXP_SECT(__data_end__))
	MIN_MMAP_END("BSS",  EXP_SECT(__bss_start__),  EXP_SECT(__bss_end__))
};

const mmap_t CODE[] = {
	MMAP_ENTRY("FLASH", 0x8000000, 0x40000, IMAGE)
	MIN_MMAP_ENTRY("SRAM2", 0x10000000, 0x4000)
	MIN_MMAP_ENTRY("SYS MEM", 0x1fff0000, 0x7000)
	MIN_MMAP_ENTRY("OTP AREA", 0x1fff7000, 0x400)
	MIN_MMAP_ENTRY("OPT BYTES", 0x1fff7800, 0x10)
};

const mmap_t PERIPH[] = {
	MIN_MMAP_ENTRY("APB1", 0x40000000, 0x9800)
	MIN_MMAP_ENTRY("APB2", 0x40010000, 0x5800)
	MIN_MMAP_ENTRY("AHB1", 0x40020000, 0x4400)
	MIN_MMAP_ENTRY("AHB2", 0x48000000, 0x8060C00)
};

const mmap_t TOP_LEVEL[] = {
	MMAP_ENTRY("CODE", 0x0, 0x20000000, CODE)
	MIN_MMAP_ENTRY("SRAM1", 0x20000000, 0xC000)
	MIN_MMAP_ENTRY("SRAM2", 0x2000C000, 0x4000)
	MMAP_ENTRY("PERIPH", 0x40000000, 0x10060C00, PERIPH)
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

		size = curr[idx].num_children;
		curr = curr[idx].children;
	}

	print_all(curr, size);

	return SUCCESS;
}

COMMAND_ENTRY(
	"memmap", "memmap [section...]",
	"Print information about the memory layout of this device.", do_memmap
)

const char *units[] = { "b", "Kb", "Mb", "Gb" };
const int units_len = sizeof(units) / sizeof(char *);

void print_mmap_t(const mmap_t *entry) {

	int i;
	const char *unit;
	uint32_t size;

	size = (entry->end_addr) ? entry->end_addr - entry->addr : entry->size;

	for (i = 0; i < units_len; i++) {
		if (size >= 1024 && !(size % 1024))
			size /= 1024;
		else break;
	}

	unit = units[i];

	printf(
		"%-10s:\t0x%08lx - 0x%08lx (%lu %s)\r\n",
		entry->name, entry->addr, entry->addr + entry->size - 1,
		size, unit
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
