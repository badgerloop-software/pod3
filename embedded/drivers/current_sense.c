#include <stdio.h>
#include <adc.h>
#include <stdlib.h>
#include <current_sense.h>

extern ADC_HandleTypeDef adc_handle;
extern uint8_t ticks;

const uint32_t SOC_INITIAL = 1013760;

uint32_t last_batt_timestamp;
uint8_t soc; 
uint8_t curr_draw, charge_perc, time_remaining;

int current_sense_init(void){
    
    int retval = 0;

    ADC_ChannelConfTypeDef sConfig6;

    /* ADC Channel Configuration */
    sConfig6.Channel = ADC_CHANNEL_11; /* Sampled channel number */
    sConfig6.Rank         = ADC_REGULAR_RANK_1; 
    sConfig6.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
    sConfig6.SingleDiff   = ADC_SINGLE_ENDED;  /* Single-ended input channel */
    sConfig6.OffsetNumber = ADC_OFFSET_NONE; /* No offset subtraction */
    sConfig6.Offset = 0;

    retval = HAL_ADC_ConfigChannel( &adc_handle, &sConfig6 );
    
    if (retval != HAL_OK){ /* Channel Configuration Error */
        printf( "Channel 11 Configuration Error\r\n");
        return retval;
    }
    return retval;
}


double current_sense_read(void){
    double analog = adc_read();
    printf("Analog Value C: %f\r\n", analog);
    return analog;
	//return (double)(adc_read( CURRENT_SENSE_PIN ) * (double)(24.414) / 1000);
}

void state_of_charge_init(void){
	soc = SOC_INITIAL;
}

void state_of_charge(void){
        if ((!ticks % 1000) && last_batt_timestamp != ticks){
                last_batt_timestamp = ticks;
                curr_draw = (current_sense_read() * 14.1) / 1000000;

                soc -= (curr_draw);
                set_charge_percentage(((soc * 100) / SOC_INITIAL));
        }
}

void set_charge_percentage(uint16_t state_of_charge){
	charge_perc = ((state_of_charge * 100) / SOC_INITIAL);
}

uint16_t  get_time_remaining(void){
	return ((curr_draw == 0) ? 1012651 : soc / curr_draw);
}	             
