#include "rcc.h"
#include <stdint.h>
#include <stdbool.h>

const char *clk_src_strings[] = {
	"MSI", "HSI", "HSE", "PLL",
	"PLLSAI1",
	"LSI", "LSE",
	"UNKNOWN", "CLK_NONE"
};

bool rcc_setClk(clk_src_t clk, bool state) {
	uint32_t set_mask, ready_mask, result;
	__IO uint32_t *reg;

    ready_mask = 0;

	/* don't try to modify the system clock source */
	if (clk == rcc_get_SysClockSrc())
		return false;
	
	switch (clk) {
		case MSI:
			reg = &RCC->CR;
			set_mask = RCC_CR_MSION;
			ready_mask = RCC_CR_MSIRDY;
			break;
		
		case HSI:
			reg = &RCC->CR;
			set_mask = RCC_CR_HSION;
			ready_mask = RCC_CR_HSIRDY;
			break;
		
		case HSE:
			reg = &RCC->CR;
			set_mask = RCC_CR_HSEON;
			ready_mask = RCC_CR_HSERDY;
		
			/* if setting, set bypass bit first */
			if (state)
				*reg = (HSE_BYP) ? 
					(*reg | RCC_CR_HSEBYP) : 
					(*reg & ~RCC_CR_HSEBYP);
			break;
			
		case PLL:
			reg = &RCC->CR;
			set_mask = RCC_CR_PLLON;
			ready_mask = RCC_CR_PLLRDY;
			break;
		
		case LSI:
			reg = &RCC->CSR;
			set_mask = RCC_CSR_LSION;
			ready_mask = RCC_CSR_LSIRDY;
			break;
		
		case LSE:
			reg = &RCC->BDCR;
			set_mask = RCC_BDCR_LSEON;
			set_mask = RCC_BDCR_LSERDY;
		
			/* if setting, set bypass bit first */
			if (state)
				*reg = (LSE_BYP) ? 
					(*reg | RCC_BDCR_LSEBYP) : 
					(*reg & ~RCC_BDCR_LSEBYP);
			break;
			
		default: return false;
	}
	
	/* set or clear the appropriate bit */
	*reg = (state) ?
		(*reg | set_mask) : 
		(*reg & ~set_mask);
	
	do {
		result = (state) ?
			(*reg & ready_mask) :	/* turn on  = wait for bit to be set	*/
			!(*reg & ready_mask);	/* turn off = wait for bit to cleared	*/
	} while(!result);

	/* write this bit if we just turned on the PLL */
	if ((clk == PLL) && state) RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
	
	return true;
}

int rcc_getHSEBYP(void) { return RCC->CR & RCC_CR_HSEBYP; }
int rcc_getLSEBYP(void) { return RCC->BDCR & RCC_BDCR_LSEBYP; }

int rcc_getClockState(clk_src_t clk) {
	switch (clk) {
		case MSI: return RCC->CR & RCC_CR_MSIRDY; break;
		case HSI: return RCC->CR & RCC_CR_HSIRDY; break;
		case HSE: return RCC->CR & RCC_CR_HSERDY; break;
		case PLL: return RCC->CR & RCC_CR_PLLRDY; break;
		case PLLSAI1: return RCC->CR & RCC_CR_PLLSAI1RDY; break;
		case LSI: return RCC->CSR & RCC_CSR_LSIRDY; break;
		case LSE: return RCC->BDCR & RCC_BDCR_LSERDY; break;
		default: return -1;
	}
	return -1;
}

bool rcc_changeSysClockSrc(clk_src_t clk) {
	uint32_t set_mask, ready_mask;

	/* make sure this source is on */
	if (!rcc_getClockState(clk))
		rcc_setClk(clk, true);
	
	switch (clk) {
		case MSI:
			set_mask =		RCC_CFGR_SW_MSI;
			ready_mask = 	RCC_CFGR_SWS_MSI;
		    break;
        case HSI:
			set_mask =		RCC_CFGR_SW_HSI;
			ready_mask =	RCC_CFGR_SWS_HSI;
			break;
		case HSE:
			set_mask =		RCC_CFGR_SW_HSE;
			ready_mask =	RCC_CFGR_SWS_HSE;
			break;
		case PLL:
			set_mask =		RCC_CFGR_SW_PLL;
			ready_mask =	RCC_CFGR_SWS_PLL;
			break;
		default: return false;
	}

	RCC->CFGR |= set_mask;
	while (!(RCC->CFGR & ready_mask)) {;}
	SystemCoreClockUpdate();
	return true;
}

clk_src_t rcc_get_SysClockSrc(void) {
	switch (RCC->CFGR & RCC_CFGR_SWS_Msk) {
		case RCC_CFGR_SWS_MSI: return MSI;
		case RCC_CFGR_SWS_HSI: return HSI;
		case RCC_CFGR_SWS_HSE: return HSE;
		case RCC_CFGR_SWS_PLL: return PLL;
	}
	return UNKNOWN;
}

clk_src_t rcc_get_PLLClockSrc(void) {
	switch (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) {
		case 0: return CLK_NONE;
		case RCC_PLLCFGR_PLLSRC_MSI: return MSI;
		case RCC_PLLCFGR_PLLSRC_HSI: return HSI;
		case RCC_PLLCFGR_PLLSRC_HSE: return HSE;
	}
	return UNKNOWN;
}

uint32_t rcc_get_MSI(void) {

	uint32_t msirange;

	/* determine source of scalar for MSI */
	if (!(RCC->CR & RCC_CR_MSIRGSEL))
		msirange = (RCC->CSR & RCC_CSR_MSISRANGE) >> 8U;
	else msirange = (RCC->CR & RCC_CR_MSIRANGE) >> 4U;

	/*MSI frequency range in HZ*/
	return MSIRangeTable[msirange];
}

uint32_t rcc_get_APB1(void) {
	return (
		rcc_get_AHB() >> APBPrescTable[
			(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos
		]
	);
}

int rcc_set_APB1(uint8_t value) {
	uint32_t curr;
	if (value > 7) return 1;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_PPRE2;
	curr |= value << RCC_CFGR_PPRE2_Pos;
	RCC->CFGR = curr;
	return 0;
}

uint32_t rcc_get_APB2(void) {
	return (
		rcc_get_AHB() >> APBPrescTable[
			(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos
		]
	);
}

int rcc_set_APB2(uint8_t value) {
	uint32_t curr;
	if (value > 7) return 1;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_PPRE1;
	curr |= value << RCC_CFGR_PPRE1_Pos;
	RCC->CFGR = curr;
	return 0;
}

uint32_t rcc_get_AHB(void) {
	return (
		SystemCoreClock >> AHBPrescTable[
			(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos
		]
	);
}

int rcc_set_AHB(uint8_t value) {
	uint32_t curr;
	if (value > 15) return 1;
	curr = RCC->CFGR;
	curr &= ~RCC_CFGR_HPRE;
	curr |= value << RCC_CFGR_HPRE_Pos;
	RCC->CFGR = curr;
	return 0;
}

int rcc_check_PLL_config(
	clk_src_t src, uint8_t N, uint8_t M, uint8_t PDIV, uint8_t R, uint8_t Q
) {

	uint32_t frequency;

	/* validate inputs */
	if (src != MSI && src != HSI && src != HSE)
		return 1;
	if (M > 7 || N > 86 || N < 8 || Q > 3 || PDIV > 31 || R > 3 || PDIV == 1)
		return 1;

	/* set frequency to what the input will be */
	switch (src) {
	case MSI: frequency = rcc_get_MSI(); break;
	case HSE: frequency = HSE_VALUE; break;
	default: frequency = 16000000;
	}

	frequency /= M + 1; 

	/* make sure M falls in a valid range */
	if (frequency < 4000000 || frequency > 16000000)
		return 1;

	/* set frequency to what the input will be */
	switch (src) {
	case MSI: frequency = rcc_get_MSI(); break;
	case HSE: frequency = HSE_VALUE; break;
	default: frequency = 16000000;
	}

	frequency /= M + 1;

	/* make sure M falls in a valid range */
	if (frequency < 4000000 || frequency > 16000000)
		return 1;

	frequency *= N;

	/* make sure N falls in a valid range */
	if (frequency < 64000000 || frequency > 344000000)
		return 1;

	/* make sure Q and R falls in a valid range */
	if ((frequency / (2 + 2*Q)) > 80000000)
		return 1;
	if ((frequency / (2 + 2*R))  > 80000000)
		return 1;

	return 0;
}

int rcc_setPLLs(
	clk_src_t src, uint8_t N, uint8_t M, uint8_t PDIV, uint8_t R, uint8_t Q
) {

	if (rcc_check_PLL_config(src, N, M, PDIV, R, Q))
		return 1;

	/* change off of PLL if we're using it */
	if (rcc_get_SysClockSrc() == PLL)
		rcc_changeSysClockSrc(MSI);

	/* turn off PLL */
	if (rcc_getClockState(PLL))
		rcc_setClk(PLL, false);

	/* turn on the input if necessary */
	if (!rcc_getClockState(src))
		rcc_setClk(src, true);

	/* reset the configuration register */
	RCC->PLLCFGR = 0x00001000;

	/* set all bits at once */
	RCC->PLLCFGR = (
		(M << RCC_PLLCFGR_PLLM_Pos) | (N << RCC_PLLCFGR_PLLN_Pos) |
		(Q << RCC_PLLCFGR_PLLQ_Pos) | (R << RCC_PLLCFGR_PLLR_Pos) |
		(PDIV << RCC_PLLCFGR_PLLPDIV_Pos) | ((uint32_t) src + 1)
	);

	return 0;
}
