#include <stdio.h>
#include "adc.h"
#include <stdlib.h>
#include "voltage_sense.h"

//TODO: This goes on Nav rather than dash

//Voltage sense is on PA4, A3, ADC1 channel 9

extern ADC_HandleTypeDef adc_handle;

int voltage_sense_init(void){
    
    int retval = 0;

    ADC_ChannelConfTypeDef sConfig4;

    /* ADC Channel Configuration */
    sConfig4.Channel      = ADC_CHANNEL_9; /* Sampled channel number */
    sConfig4.Rank         = ADC_REGULAR_RANK_1; 
    sConfig4.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
    sConfig4.SingleDiff   = ADC_SINGLE_ENDED;  /* Single-ended input channel */
    sConfig4.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
    sConfig4.Offset       = 0;

    retval = HAL_ADC_ConfigChannel( &adc_handle, &sConfig4 );
    
    if (retval != HAL_OK){ /* Channel Configuration Error */
        printf( "Channel 9 Configuration Error\r\n");
        return retval;
    }

    return retval;
}

uint32_t voltage_sense_read(void){
    return adc_read();
}

