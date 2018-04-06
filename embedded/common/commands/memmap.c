#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "memmap.h"

#define EXP_SECT(sect)\
	((const uint32_t) &sect)

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

const mmap_t APB1[] = {
	MIN_MMAP_ENTRY("LPTIM2", 0x40009400, 0x400)
	MIN_MMAP_ENTRY("SWPMI1", 0x40008800, 0x400)
	MIN_MMAP_ENTRY("LPUART1", 0x40008000, 0x400)
	MIN_MMAP_ENTRY("LPTIM1", 0x40007C00, 0x400)
	MIN_MMAP_ENTRY("OPAMP", 0x40007800, 0x400)
	MIN_MMAP_ENTRY("DAC", 0x40007400, 0x400)
	MIN_MMAP_ENTRY("PWR", 0x40007000, 0x400)
	MIN_MMAP_ENTRY("USB SRAM", 0x40006C00, 0x400)
	MIN_MMAP_ENTRY("USB FS", 0x40006800, 0x400)
	MIN_MMAP_ENTRY("CAN1", 0x40006400, 0x400)
	MIN_MMAP_ENTRY("CRS", 0x40006000, 0x400)
	MIN_MMAP_ENTRY("I2C3", 0x40005C00, 0x400)
	MIN_MMAP_ENTRY("I2C1", 0x40005400, 0x400)
	MIN_MMAP_ENTRY("USART2", 0x40004400, 0x400)
	MIN_MMAP_ENTRY("SPI3", 0x40003C00, 0x400)
	MIN_MMAP_ENTRY("IWDG", 0x40003000, 0x400)
	MIN_MMAP_ENTRY("WWDG", 0x40002C00, 0x400)
	MIN_MMAP_ENTRY("RTC", 0x40002800, 0x400)
	MIN_MMAP_ENTRY("TIM7", 0x40001400, 0x400)
	MIN_MMAP_ENTRY("TIM6", 0x40001000, 0x400)
	MIN_MMAP_ENTRY("TIM2", 0x40000000, 0x400)
};

const mmap_t APB2[] = {
	MIN_MMAP_ENTRY("SAI1", 0x40015400, 0x400)
	MIN_MMAP_ENTRY("TIM16", 0x40014400, 0x400)
	MIN_MMAP_ENTRY("TIM15", 0x40014000, 0x400)
	MIN_MMAP_ENTRY("USART1", 0x40013800, 0x400)
	MIN_MMAP_ENTRY("SPI1", 0x40013000, 0x400)
	MIN_MMAP_ENTRY("TIM1", 0x40012C00, 0x400)
	MIN_MMAP_ENTRY("FIREWALL", 0x40011C00, 0x400)
	MIN_MMAP_ENTRY("EXTI", 0x40010400, 0x400)
	MIN_MMAP_ENTRY("COMP", 0x40010200, 0x400)
	MIN_MMAP_ENTRY("SYSCFG", 0x40010000, 0x400)
};

const mmap_t AHB1[] = {
	MIN_MMAP_ENTRY("TSC", 0x40024000, 0x400)
	MIN_MMAP_ENTRY("CRC", 0x40023000, 0x400)
	MIN_MMAP_ENTRY("FLASH registers", 0x40022000, 0x400)
	MIN_MMAP_ENTRY("RCC", 0x40021000, 0x400)
	MIN_MMAP_ENTRY("DMA2", 0x40020400, 0x400)
	MIN_MMAP_ENTRY("DMA1", 0x40020000, 0x400)
};

const mmap_t AHB2[] = {
	MIN_MMAP_ENTRY("RNG", 0x50060800, 0x400)
	MIN_MMAP_ENTRY("ADC", 0x50040000, 0x400)
	MIN_MMAP_ENTRY("GPIOH", 0x48001C00, 0x400)
	MIN_MMAP_ENTRY("GPIOC", 0x48000800, 0x400)
	MIN_MMAP_ENTRY("GPIOB", 0x48000400, 0x400)
	MIN_MMAP_ENTRY("GPIOA", 0x48000000, 0x400)
};

const mmap_t PERIPH[] = {
	MMAP_ENTRY("APB1", 0x40000000, 0x9800, APB1)
	MMAP_ENTRY("APB2", 0x40010000, 0x5800, APB2)
	MMAP_ENTRY("AHB1", 0x40020000, 0x4400, AHB1)
	MMAP_ENTRY("AHB2", 0x48000000, 0x8060C00, AHB2)
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
		"%-10s:\t0x%08lx - 0x%08lx (%lu %s) %s\r\n",
		entry->name, entry->addr,
		(entry->end_addr) ? entry->end_addr - 1 : entry->addr + entry->size - 1,
		size, unit, (entry->children) ? "*" : ""
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
