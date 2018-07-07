#include <stdio.h>
#include "rcc.h"
#include "stm32l432xx.h"
#include "i2c.h"

const unsigned int *UID = (const unsigned int *) UID_BASE;

void print_rst_str(void) {

	int first = 1;

	fputs("Reset bit(s): ", stdout);

	if (RCC->CSR & RCC_CSR_LPWRRSTF) {
		fputs("low-power entry error", stdout);
	}
	if (RCC->CSR & RCC_CSR_WWDGRSTF) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("window watchdog", stdout);
	}
	if (RCC->CSR & RCC_CSR_IWDGRSTF) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("independent window watchdog", stdout);
	}
	if (RCC->CSR & RCC_CSR_SFTRSTF) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("software", stdout);
	}
	if (RCC->CSR & RCC_CSR_BORRSTF ) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("brown-out", stdout);
	}
	if (RCC->CSR & RCC_CSR_PINRSTF) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("NRST pin", stdout);
	}
	if (RCC->CSR & RCC_CSR_OBLRSTF) {
		if (!first) fputs(", ", stdout);
		else first = 0;
		fputs("option-byte loader", stdout);
	}
	if (RCC->CSR & RCC_CSR_FWRSTF) {
		if (!first) fputs(", ", stdout);
		fputs("firewall", stdout);
	}

	fputs("\r\n", stdout);
}

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
	print_rst_str();
	printf("Frequencies:\r\n");
	printf("SYSCLK:\t%lu kHz (source: %s)\r\n", SystemCoreClock / 1000, clk_src_strings[rcc_get_SysClockSrc()]);
	printf("HCLK:\t%lu kHz\r\n", rcc_get_AHB() / 1000);
	printf("APB1:\t%lu kHz\r\n", rcc_get_APB1() / 1000);
	printf("APB2:\t%lu kHz\r\n", rcc_get_APB2() / 1000);
	printf("I2C Devices:\r\n");
	int i;
	for(i = 0; i < 128; i++) i2c_scan(i);
	puts("----------------------------------------------------------------------\r");

	/* clear reset flags */
	RCC->CSR |= RCC_CSR_RMVF;
}
