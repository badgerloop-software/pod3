#include <stdio.h>
#include <string.h>
#include "commands.h"

#include "pin_alias.h"

void gpio_printPinInfoHeader(void) {
	printf("\r\nTYPE\tNAME\t\tPORTPIN\tCFG\tSPEED\tPU/D\r\n");
	printf("====================================================\r\n");
}

void gpio_printPinInfo(GPIO_TypeDef* port, uint8_t pin) {

	GPIO alias;

	if (getGpioAlias(&port, &pin, &alias)) {
		if (alias.usable) printf("USER");
		else printf("SYS ");
		printf("\t%-10s", alias.name);
	}
	else printf("NONE\t-\t");

	printf("\t%c%2d\t", gpio_getGpioPortChar(port), pin);

	switch (gpio_getMode(port, pin)) {
		case OUTPUT:	printf("out %x", gpio_readPin(port, pin)); break;
		case INPUT:		printf("in  %x", gpio_readPin(port, pin)); break;
		case ALT:		printf("af%2d", gpio_getAlternateFunc(port, pin));
							break;
		case ANALOG:	printf("ana"); break;
		default:		printf("UNKNOWN");
	}

	putchar('\t');
	switch (gpio_getSpeed(port, pin)) {
		case LOW_SPEED:			printf("low"); break;
		case MEDIUM_SPEED:		printf("medium"); break;
		case HIGH_SPEED:		printf("high"); break;
		case VERY_HIGH_SPEED:	printf("vhigh"); break;
		default:				printf("UNKNOWN");
	}

	putchar('\t');
	switch (gpio_getPullupState(port, pin)){
		case NONE:		printf("none"); break;
		case PULL_UP:	printf("up"); break;
		case PULL_DOWN:	printf("down"); break;
		default:		printf("UNKNOWN");
	}
	putchar('\r'); putchar('\n');
}

command_status do_io(int argc, char *argv[]) {

	GPIO_TypeDef *port;
	uint8_t pin = -1;
	int i;

	if (argc < 2)
		return USAGE;

	if (!strcmp("aliases", argv[1])) {
		for (i = 0; i < NUM_GROUPS; i++)
			printf("%s\r\n", GROUP_NAMES[i]);
		return SUCCESS;
	}

	port = gpio_getGpioPort(argv[1][0]);

	/* check valid GPIO address */
	if (!IS_GPIO_ALL_INSTANCE(port)) return USAGE;

	if (argc >= 3)
		pin = argv[2][0] - '0';

	gpio_printPinInfoHeader();

	/* print single pin */
	if (pin <= 15) {
		gpio_printPinInfo(port, pin);
		putchar('\r'); putchar('\n');
		return SUCCESS;
	}

	/* print all pins */
	for (i = 0; i < 15; i++) {
		gpio_printPinInfo(port, i);
	}

	putchar('\r'); putchar('\n');

	return SUCCESS;
}

COMMAND_ENTRY(
	"io",
	"io port [pin]",
	"TODO.",
	do_io
)
