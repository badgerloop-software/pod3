#include <stdio.h>
#include "adc.h"
#include <stdlib.h>
#include "voltage_sense.h"

extern ADC_HandleTypeDef adc_handle;

int voltage_sense_init(void){
    
    int retval = 0;

    ADC_ChannelConfTypeDef sConfig5;

    /* ADC Channel Configuration */
    sConfig5.Channel = ADC_CHANNEL_10; /* Sampled channel number */
    sConfig5.Rank         = ADC_REGULAR_RANK_1; 
    sConfig5.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
    sConfig5.SingleDiff   = ADC_SINGLE_ENDED;  /* Single-ended input channel */
    sConfig5.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
    sConfig5.Offset = 0;

    retval = HAL_ADC_ConfigChannel( &adc_handle, &sConfig5 );
    
    if (retval != HAL_OK){ /* Channel Configuration Error */
        printf( "Channel 10 Configuration Error\r\n");
        return retval;
    }

    return retval;
}

double voltage_sense_read(void){
    return (double)(adc_read( VOLTAGE_SENSE_PIN ) * 1000 / (double)(0.80566));
}

