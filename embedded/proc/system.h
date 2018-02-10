#ifndef __SYSTEM_STM32L4XX_H
#define __SYSTEM_STM32L4XX_H

#include "stm32l432xx.h"

#define HSE_VALUE	8000000U  /* Value of the External oscillator in Hz */
#define MSI_VALUE	4000000U  /* Value of the Internal oscillator in Hz */
#define HSI_VALUE	16000000U /* Value of the Internal oscillator in Hz */

#define LED3_PIN	3
#define LED_PORT	GPIOB

/* Uncomment the following line if you need to relocate your vector Table in
   Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /* Vector Table base offset field.
                                 This value must be a multiple of 0x200. */

void SystemCoreClockUpdate(void);

#endif
