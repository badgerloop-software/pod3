#ifndef _ADC__H__
#define _ADC__H__

//#include "stm32l4xx_hal_conf.h"
#include "stm32l4xx_hal_adc.h"
//#include "stm32l4xx_hal_adc_ex.h"
//#include "stm32l4xx_ll_adc.h"

HAL_StatusTypeDef adc_init(void);
uint32_t adc_read( );
HAL_StatusTypeDef adc_start(void);

/* ADC Info */
//PA5 is CURR_SENSE, and is connected to ADC1/10
//PA6 is VOLT_SENSE, and is connected to ADC1/11
ADC_HandleTypeDef adc_handle;
extern ADC_ChannelConfTypeDef sConfig5;
extern ADC_ChannelConfTypeDef sConfig6;

#endif
