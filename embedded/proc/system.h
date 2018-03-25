#ifndef __SYSTEM_STM32L4XX_H
#define __SYSTEM_STM32L4XX_H

#include "stm32l432xx.h"

#define HSE_VALUE	8000000U  /* Value of the External oscillator in Hz */
#define MSI_VALUE	4000000U  /* Value of the Internal oscillator in Hz */
#define HSI_VALUE	16000000U /* Value of the Internal oscillator in Hz */
#define LSE_VALUE	32768U

#define USB_UART	USART2

/* Uncomment the following line if you need to relocate your vector Table in
   Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /* Vector Table base offset field.
                                 This value must be a multiple of 0x200. */

void SystemCoreClockUpdate(void);
void delay_ms(unsigned int period);

extern volatile unsigned int ticks;
extern uint32_t SystemCoreClock;

extern const uint8_t AHBPrescTable[16];
extern const uint8_t APBPrescTable[8];
extern const uint32_t MSIRangeTable[12];

#endif
