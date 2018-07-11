#include "adc.h"
#include "stm32l432xx.h"

#include "gpio.h"
#include "board.h"
#include "usart.h"
#include "rcc.h"
#include "pin_alias.h"
#include "console.h"

#define APB2_F 24000000

const ADC_INFO ADC_LUT[] = {
    FILL_ADC(ADC1, 10,  GPIOA, 5)  /* PA5: -  */
    FILL_ADC(ADC1, 11,  GPIOA, 6)  /* PA6: -  */
};

#define AVERAGE_SIZE 100
int filter[2][16][AVERAGE_SIZE];
uint32_t count[2][16];
int average[2][16];

uint32_t adcFreq = 0;
ADC_TypeDef adc_handle;
ADC_Common_TypeDef *adc1_common = ADC1_COMMON;

const uint8_t NUM_ADC = sizeof(ADC_LUT)/sizeof(ADC_INFO);

static void adc_init_sampling(void) {
	int i, bitsToShift;
	__IO uint32_t *curr_reg;

	/* Initialize Sampling Cycles */
	adc_handle.SMPR1 = 0x0; adc_handle.SMPR2 = 0x0;
	curr_reg = &(adc_handle.SMPR2);
	bitsToShift = 0;
	for (i = 0; i < 18; i++) {
		if (i == 10) {
			curr_reg = &(adc_handle.SMPR1);
			bitsToShift = 0;
		}
		*curr_reg |= ADC_SMP_BITS << bitsToShift;
		bitsToShift += 3;
	}
}

static bool adc_init_clk(void) {
	uint8_t divisor = 2;

	/*enable clock in RCC register */
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	if(adcFreq == 0){
		while (divisor <= 8 && ((APB2_F / divisor) > ADC_TARGET_FREQ))
			divisor += 2;
		if (divisor > 8) return false;
		adcFreq = APB2_F / divisor;
		adc1_common->CCR &= ~ADC_CCR_PRESC_Msk;
		switch (divisor) {
			case 4: adc1_common->CCR |= ADC_CCR_PRESC_0; break;
			case 6: adc1_common->CCR |= ADC_CCR_PRESC_1; break;
			case 8: adc1_common->CCR |= ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0; break;
		}
	}

	adc_init_sampling();

	return true;
}

uint16_t analogRead(uint8_t channel) {
	uint16_t retval = 0;

	/* single conversion */
	adc_handle.SQR1 = 0x0; adc_handle.SQR3 = 0x0;

	/* set channel */
	adc_handle.SQR3 |= channel & 0x1f;

	/* start conversion */
	adc_handle.CR |= ADC_CR_ADSTART;

	/* wait till end */
	while (!(adc_handle.ISR & ADC_ISR_EOC)) {;}

	/* read data */
	retval = adc_handle.DR;

	/* turn off ADC */
	//adc->CR2 &= ~ADC_CR2_ADON;

	average[1][channel] -= filter[1][channel][count[1][channel] % AVERAGE_SIZE];
	average[1][channel] += retval;
	filter[1][channel][count[1][channel] % AVERAGE_SIZE] = retval;

	count[1][channel]++;

	/* return data */
	return average[1][channel] / AVERAGE_SIZE;
}


bool adc_init(void) {
	uint32_t curr_tick = (uint32_t) ticks;
	uint8_t i, ii;

	/* initialize if necessary */
	if (!adc_init_clk())
		return false;

	adc_handle.CR = 0x0; 
	adc_handle.CR |= ADC_CFGR_RES_0; /* 10-bit accuracy */
	adc_handle.CR |= 1;
	while (curr_tick == ticks) {;} /* stabilization? */
	for (i = 0; i < 16; i++) {
		for(ii = 0; ii < AVERAGE_SIZE; ii++){
			analogRead(i);
		}
	}

	return true;
}

