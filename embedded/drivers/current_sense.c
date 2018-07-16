#include <stdio.h>
#include "adc.h"
#include <stdlib.h>
#include "current_sense.h"
#include "stm32l4xx_hal_adc.h"

extern ADC_HandleTypeDef adc_handle;

//Current Sensor is on pin PA5, A4, ADC1 channel 10 10 10 10 10 10 10 10 10 

int current_sense_init(void){
    
    int retval = 0;

    ADC_ChannelConfTypeDef sConfig6;

    /* ADC Channel Configuration */
    sConfig6.Channel      = ADC_CHANNEL_10; /* Sampled channel number */
    sConfig6.Rank         = ADC_REGULAR_RANK_1; 
    sConfig6.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
    sConfig6.SingleDiff   = ADC_SINGLE_ENDED;  /* Single-ended input channel */
    sConfig6.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
    sConfig6.Offset       = 0;

    retval = HAL_ADC_ConfigChannel( &adc_handle, &sConfig6 );
    
    if (retval != HAL_OK){ /* Channel Configuration Error */
        printf( "Channel 11 Configuration Error\r\n");
        return retval;
    }
    return retval;
}

uint32_t current_sense_read(void){
    return adc_read();
}

