#include <stdio.h>
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_hal_conf.h"
#include "adc.h"

/* ADC Info */
//PA5 is CURR_SENSE, and is connected to ADC1/10
////PA6 is VOLT_SENSE, and is connected to ADC1/11
ADC_ChannelConfTypeDef sConfig6;

HAL_StatusTypeDef adc_start(void){
	
    HAL_StatusTypeDef retval;

	/* Run the ADC calibration in single-ended mode */
	retval = HAL_ADCEx_Calibration_Start(&adc_handle, ADC_SINGLE_ENDED);
  	if (retval != HAL_OK) {
     		/* Calibration Error */
   		printf( "ADC Calibration Error\r\n");
   		return retval;
   	}
	

  	/*##-3- Start the conversion process #######################################*/
	retval = HAL_ADC_Start(&adc_handle);
	if (retval != HAL_OK) {
     		/* Start Conversation Error */
   		printf( "Start Conversion Error\r\n");
   		return retval;
   	}

	return retval;
}

HAL_StatusTypeDef adc_init(void){
	HAL_StatusTypeDef retval;
	adc_handle.Instance          = ADC1;
	retval = HAL_ADC_DeInit(&adc_handle);
   	if (retval != HAL_OK) {
		/* ADC de-initialization Error */
		printf( "ADC De-Init Error\r\n");
		return retval;
	}

	__HAL_RCC_ADC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

    adc_handle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */ 
   	adc_handle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */ 
   	adc_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */ 
   	adc_handle.Init.ScanConvMode          = ENABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */ 
   	adc_handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */ 
   	adc_handle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */ 
  	adc_handle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */ 
   	adc_handle.Init.NbrOfConversion       = 2;                             /* Parameter discarded because sequencer is disabled */ 
   	adc_handle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */ 
 	adc_handle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	adc_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */ 
   	adc_handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */ 
   	adc_handle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */ 
   	adc_handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */ 
   	adc_handle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */ 

	retval = HAL_ADC_Init(&adc_handle);
	if (retval != HAL_OK) {
		/* ADC initialization Error */
		printf( "ADC Initialization Error\r\n");
   		return retval;
   	}

    return HAL_OK;
}


uint32_t adc_read( void  ){
    uint32_t ADCConvertedValue = 0;

    if (HAL_ADC_PollForConversion(&adc_handle, 1000 ) != HAL_OK){
		/* End Of Conversion flag not set on time */
	   	printf("ADC End of conversion error.\r\n");
	} else {
  		/* ADC conversion completed */
		/*##-5- Get the converted value of regular channel  #########*/
		ADCConvertedValue = HAL_ADC_GetValue(&adc_handle);
	}
	return ADCConvertedValue;
}
