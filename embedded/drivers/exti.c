#include "exti.h"
#include <string.h>
#include "stm32l432xx.h"
#include <stdio.h>

int count1 = 0;
int count2 = 0;
int count3 = 0;

/* gets interupt vecotor number for pin */
static IRQn_Type exti_get_irq_num(uint32_t pin) {
	switch(pin) {
		case 0: return EXTI0_IRQn;
		case 1: return EXTI1_IRQn;
		case 2: return EXTI2_IRQn;
		case 3:  return EXTI3_IRQn;
		case 4:  return EXTI4_IRQn;
		case 5 ... 9: return EXTI9_5_IRQn;
		case 10 ... 15: return EXTI15_10_IRQn;
		default: return SysTick_IRQn;	/* -1 */
	}
}

/*sets up an interrupt from a passed in port and pin */
void exti_config(GPIO_TypeDef * port, uint32_t pin, bool rtsr, bool ftsr, bool ie) {
	
	interLine[pin].prev = 0;
	interLine[pin].curr = 0;
	interLine[pin].count = 0;
	memset(interLine[pin].filter, 0x00, AVERAGE_SIZE*sizeof(uint32_t));

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[pin / 4] &= ~(7 << (pin % 4 * 4)); 	/* clearing SYSCFG external interrupt configuration registers */
	SYSCFG->EXTICR[pin / 4] |= (gpio_getGpioPortChar(port) - 65) << (pin % 4 * 4); 	/* SYSCFG external interrupt configuration registers */
	
	EXTI->PR1 = 0xffff;
	EXTI->IMR1 |= 0x1U << pin;

	if (rtsr)
		EXTI->RTSR1 |= 0x1U << pin;		/* rising trigger selection register */
	else
		EXTI->RTSR1 &= ~(0x1U << pin);    	
	if (ftsr)
		EXTI->FTSR1 |= 0x1U << pin;       /* falling trigger selection register */
	else
		EXTI->FTSR1 &= ~(0x1U << pin);

	if (ie) {
		NVIC_SetPriority(exti_get_irq_num(pin), 4);
		NVIC_EnableIRQ(exti_get_irq_num(pin));
	}
}

/*disable interrupt from given pin */
void exti_disable(GPIO_TypeDef * port, uint32_t pin) {
		RCC->APB2ENR |= (1 << 14); 
		SYSCFG->EXTICR[pin/4] &= ~((gpio_getGpioPortChar(port)-65) << (pin%4*4)); 	/* SYSCFG external interrupt configuration registers */
		EXTI->IMR1 &= ~(0x1U << pin);
}

/*gets number of interrupts from given pin*/
timeStamp * getTimeStamps(int pin) {
	return &interLine[pin];
}

/*need to get time stamps*/
void EXTI0_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF0) {
		interLine[0].prev = interLine[0].curr;
		interLine[0].curr = ticks;
		interLine[0].filter[(interLine[0].count) % AVERAGE_SIZE]
			= interLine[0].curr - interLine[0].prev;
		interLine[0].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF0;
		
		/* TESTING */
        count1++;
        printf("Retro 1 Count: %d\r\n", count1);
        /* END TESTING */
	}
}

void EXTI1_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF1) {
		interLine[1].prev = interLine[1].curr;
		interLine[1].curr = ticks;
		interLine[1].filter[(interLine[1].count) % AVERAGE_SIZE]
			= interLine[1].curr - interLine[1].prev;
		interLine[1].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF1;
		
		/* TESTING */
        count2++;
        printf("Retro 2 Count: %d\r\n", count2 );
        /* END TESTING */
	}
}

void EXTI2_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF2) {
		interLine[2].prev = interLine[2].curr;
		interLine[2].curr = ticks;
		interLine[2].filter[(interLine[2].count) % AVERAGE_SIZE]
			= interLine[2].curr - interLine[2].prev;
		interLine[2].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF2;
	}
}

void EXTI3_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF3) {
		interLine[3].prev = interLine[3].curr;
		interLine[3].curr = ticks;
		interLine[3].filter[(interLine[3].count) % AVERAGE_SIZE]
			= interLine[3].curr - interLine[3].prev;
		interLine[3].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF3;
	
	}
}

void EXTI4_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF4) {
		interLine[4].prev = interLine[4].curr;
		interLine[4].curr = ticks;
		interLine[4].filter[(interLine[4].count) % AVERAGE_SIZE]
			= interLine[4].curr - interLine[4].prev;
		interLine[4].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF4;
	}
}

void EXTI9_5_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF5) {
		interLine[5].prev = interLine[5].curr;
		interLine[5].curr = ticks;
		interLine[5].filter[(interLine[5].count) % AVERAGE_SIZE]
			= interLine[5].curr - interLine[5].prev;
		interLine[5].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF5;

        /* TESTING */
        count3++;
        printf("Retro 3 Count: %d\r\n", count3 );
        /* END TESTING */
	
    }
	if (EXTI->PR1 & EXTI_PR1_PIF6) {
		interLine[6].prev = interLine[6].curr;
		interLine[6].curr = ticks;
		interLine[6].filter[(interLine[6].count) % AVERAGE_SIZE]
			= interLine[6].curr - interLine[6].prev;
		interLine[6].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF6;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF7) {
		interLine[7].prev = interLine[7].curr;
		interLine[7].curr = ticks;
		interLine[7].filter[(interLine[7].count) % AVERAGE_SIZE]
			= interLine[7].curr - interLine[7].prev;
		interLine[7].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF7;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF8) {
		interLine[8].prev = interLine[8].curr;
		interLine[8].curr = ticks;
		interLine[8].filter[(interLine[8].count) % AVERAGE_SIZE]
			= interLine[8].curr - interLine[8].prev;
		interLine[8].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF8;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF9) {
		interLine[9].prev = interLine[9].curr;
		interLine[9].curr = ticks;
		interLine[9].filter[(interLine[9].count) % AVERAGE_SIZE]
			= interLine[9].curr - interLine[9].prev;
		interLine[9].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF9;
	}
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF10) {
		interLine[10].prev = interLine[10].curr;
		interLine[10].curr = ticks;
		interLine[10].filter[(interLine[10].count) % AVERAGE_SIZE]
			= interLine[10].curr - interLine[10].prev;
		interLine[10].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF10;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF11) {
		interLine[11].prev = interLine[11].curr;
		interLine[11].curr = ticks;
		interLine[11].filter[(interLine[11].count) % AVERAGE_SIZE]
			= interLine[11].curr - interLine[11].prev;
		interLine[11].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF11;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF12) {
		interLine[12].prev = interLine[12].curr;
		interLine[12].curr = ticks;
		interLine[12].filter[(interLine[12].count) % AVERAGE_SIZE]
			= interLine[12].curr - interLine[12].prev;
		interLine[12].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF12;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF13) {
		interLine[13].prev = interLine[13].curr;
		interLine[13].curr = ticks;
		interLine[13].filter[(interLine[13].count) % AVERAGE_SIZE]
			= interLine[13].curr - interLine[13].prev;
		interLine[13].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF13;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF14) {
		interLine[14].prev = interLine[14].curr;
		interLine[14].curr = ticks;
		interLine[14].filter[(interLine[14].count) % AVERAGE_SIZE]
			= interLine[14].curr - interLine[14].prev;
		interLine[14].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF14;
	}
	if (EXTI->PR1 & EXTI_PR1_PIF15) {
		interLine[15].prev = interLine[15].curr;
		interLine[15].curr = ticks;
		interLine[15].filter[(interLine[15].count) % AVERAGE_SIZE]
			= interLine[15].curr - interLine[15].prev;
		interLine[15].count ++;

		EXTI->PR1 |= EXTI_PR1_PIF15;
	}
}
