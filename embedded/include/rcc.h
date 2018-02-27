#ifndef _RCC__H__
#define _RCC__H__

#include <stdint.h>
#include <stdbool.h>
#include "system.h"

#define HSE_BYP	1	/* current nucleo144 config needs this */
#define LSE_BYP	0
extern const char *clk_src_strings[];

#define NUM_CLK_SRCS 5
typedef enum clk_src {
	MSI = 0, HSI = 1, HSE = 2, PLL = 3,
	PLLSAI1 = 4,
	LSI = 5, LSE = 6,
	UNKNOWN = 7, NONE = 8
} clk_src_t;

/* Setting */
bool rcc_setClk(clk_src_t clk, bool state);
bool rcc_changeSysClockSrc(clk_src_t clk);
int rcc_getClockState(clk_src_t clk);

/* Getting */
clk_src_t rcc_get_SysClockSrc(void);
clk_src_t rcc_get_PLLClockSrc(void);
int rcc_getHSEBYP(void);
int rcc_getLSEBYP(void);
uint32_t rcc_get_APB1(void);
uint32_t rcc_get_APB2(void);

#endif
