#include "exti.h"
#include <string.h>

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
               default: return SysTick_IRQn;   /* -1 */
         }
}

/*sets up an interrupt from a passed in port and pin */
void exti_config(GPIO_TypeDef * port, uint32_t pin, bool rtsr, bool ftsr, bool ie) {
	interLine[pin].prev = 0;
	interLine[pin].curr = 0;
	interLine[pin].count = 0;
	memset(interLine[pin].filter, 0x00, AVERAGE_SIZE*sizeof(uint32_t));
        gpio_setMode(port, pin, INPUT);
        GPIOC->OSPEEDR |= (0x03<< (2 * pin));    // high speed
        RCC->APB2ENR |= (1 << 14);
        SYSCFG->EXTICR[pin / 4] |= (gpio_getGpioPortChar(port) - 65) << (pin % 4 * 4);  /* SYSCFG external interrupt configuration registers */
        EXTI->PR1 = 0xffff;
        EXTI->IMR1 |= 0x1U << pin;
        if (rtsr)
	        EXTI->RTSR1 |= 0x1U << pin;              /* rising trigger selection register */
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

/*gets number of interrupts from given pin */
timeStamp * getTimeStamps(int pin) {
	        return &interLine[pin];
}

