#include <stdio.h>
#include "rcc.h"
#include "stm32l432xx.h"

const unsigned int *UID = (const unsigned int *) UID_BASE;

void post(const char *app_name) {

	uint8_t sernum[3];

	sernum[0] = (UID[0] & 0xff0000) >> 16;
	sernum[1] = (UID[0] & 0xff00) >> 8;
	sernum[2] = (UID[0] & 0xff);

	puts("\r\n----------------------------------------------------------------------\r");
	printf("-                   Badgerloop Firmware (board: %x:%x:%x)             -\r\n", sernum[2], sernum[1], sernum[0]);
	printf("-     %16s Build (r%02d) %.28s      -\r\n", app_name, __VERSION, _SHELL_TIME);
	printf("-        commit: '%20s' from %.19s   -\r\n", _GIT_VERSION, _GIT_TIME);
	puts("----------------------------------------------------------------------\r");
	printf("Frequencies:\r\n");
	printf("SYSCLK:\t%lu kHz (source: %s)\r\n", SystemCoreClock / 1000, clk_src_strings[rcc_get_SysClockSrc()]);
	printf("APB1:\t%lu kHz\r\n", rcc_get_APB1() / 1000);
	printf("APB2:\t%lu kHz\r\n", rcc_get_APB2() / 1000);
	puts("----------------------------------------------------------------------\r");

}
