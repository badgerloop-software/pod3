#include "system.h"

uint32_t SystemCoreClock = 4000000U;

const uint8_t AHBPrescTable[16] = {
	0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U
};
const uint8_t APBPrescTable[8] =  {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
const uint32_t MSIRangeTable[12] = {
	100000U, 200000U, 400000U, 800000U, 1000000U, 2000000U,
	4000000U, 8000000U, 16000000U, 24000000U, 32000000U, 48000000U
};

void SystemInit(void) {
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	/* set CP10 and CP11 Full Access */
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
#endif
	RCC->CR |= RCC_CR_MSION;	/* Set MSION bit */
	RCC->CFGR = 0x00000000U;	/* Reset CFGR register */
	RCC->CR &= 0xEAF6FFFFU;		/* Reset HSEON, CSSON, HSION, and PLLON bits */
	RCC->PLLCFGR = 0x00001000U;	/* Reset PLLCFGR register */
	RCC->CR &= 0xFFFBFFFFU;		/* Reset HSEBYP bit */
	RCC->CIER = 0x00000000U;	/* Disable all interrupts */
#ifdef VECT_TAB_SRAM
	/* Vector Table Relocation in Internal SRAM */
	SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
	/* ^ FLASH */
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
#endif
	SystemCoreClockUpdate();
}

void SystemCoreClockUpdate(void) {
	uint32_t tmp = 0U, msirange = 0U, pllvco = 0U, pllr = 2U,
			 pllsource = 0U, pllm = 2U;

	/* MSISRANGE from RCC_CSR applies */
	if (!(RCC->CR & RCC_CR_MSIRGSEL))
		msirange = (RCC->CSR & RCC_CSR_MSISRANGE) >> 8U;

	/* MSIRANGE from RCC_CR applies */
	else msirange = (RCC->CR & RCC_CR_MSIRANGE) >> 4U;

	/*MSI frequency range in HZ*/
	msirange = MSIRangeTable[msirange];

	/* Get SYSCLK source */
	switch (RCC->CFGR & RCC_CFGR_SWS) {

		/* MSI used as system clock source */
		case 0x00: SystemCoreClock = msirange; break;

		/* HSI used as system clock source */
		case 0x04: SystemCoreClock = HSI_VALUE; break;

		/* HSE used as system clock source */
		case 0x08: SystemCoreClock = HSE_VALUE; break;

		case 0x0C:	/* PLL used as system clock  source */
			/* PLL_VCO = (HSE_VALUE or HSI_VALUE or MSI_VALUE/ PLLM) * PLLN
			   SYSCLK = PLL_VCO / PLLR */
			pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
			pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> 4U) + 1U ;

			switch (pllsource) {
				/* HSI used as PLL clock source */
				case 0x02: pllvco = (HSI_VALUE / pllm); break;
				/* HSE used as PLL clock source */
				case 0x03: pllvco = (HSE_VALUE / pllm); break;
				/* MSI used as PLL clock source */
				default: pllvco = (msirange / pllm); break;
			}
			pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 8U);
			pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 25U) + 1U) * 2U;
			SystemCoreClock = pllvco/pllr;
			break;

		default: SystemCoreClock = msirange; break;
	}

	/* Compute HCLK clock frequency */
	tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4U)];
	SystemCoreClock >>= tmp;

	SysTick_Config(SystemCoreClock / 1000);
}
