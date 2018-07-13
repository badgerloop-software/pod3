#ifndef __SYSTEM_STM32L4XX_H
#define __SYSTEM_STM32L4XX_H

#include "stm32l432xx.h"
#include "stm32l4xx_hal_conf.h"

//#define HSE_VALUE	(uint32_t) 8000000U  /* Value of the External oscillator in Hz */
//#define MSI_VALUE	(uint32_t) 4000000U  /* Value of the Internal oscillator in Hz */
//#define HSI_VALUE	(uint32_t) 16000000U /* Value of the Internal oscillator in Hz */
//#define LSE_VALUE	(uint32_t) 32768U

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
