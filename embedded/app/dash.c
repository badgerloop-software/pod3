#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "uart.h"
#include "dashboard_control.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_ll_adc.h"

#define BLINK_INTERVAL	250

/* ADC Info */
//PA5 is CURR_SENSE, and is connected to ADC1/10
//PA6 is VOLT_SENSE, and is connected to ADC1/11
ADC_HandleTypeDef    AdcHandle;

ADC_ChannelConfTypeDef sConfig5;
ADC_ChannelConfTypeDef sConfig6;

/* Nucleo 32 I/O */
//THERM1: between batteries 1 and 2
//THERM2: between batteries 2 and 3
FILL_GPIO(THERM1,     GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(THERM2,     GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR)

//SENSOR
//VOLT_SENSE is 12V battery voltage
//CURR_SENSE is 12V battery current draw, charging not included
FILL_GPIO(VOLT_SENSE, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(CURR_SENSE, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR)

//UART
FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER )

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )

//SD
FILL_GPIO(SD_CS, GPIOB, 0, OUTPUT, LOW_SPEED, NONE, true, OTHER )

inline void printPrompt(void) {
	fputs("[dash-build] $ ", stdout);
	fflush(stdout);
}

int adc_init(void){

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance          = ADC1;

   	if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
   	{
     		/* ADC de-initialization Error */
   		printf( "ADC De-Init Error\r\n");
   		return 1;
	}
 
   	AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */ 
   	AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */ 
   	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */ 
   	AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */ 
   	AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */ 
   	AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */ 
   	AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */ 
   	AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */ 
   	AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */ 
   	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */ 
   	AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */ 
   	AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */ 
   	AdcHandle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */ 

	if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
	{
     		/* ADC initialization Error */
   		printf( "ADC Initialization Error\r\n");
   		return 1;
   	}
 
   	/*##-2- Configure ADC regular channel ######################################*/

   	sConfig5.Channel      = ADC_CHANNEL_10;                /* Sampled channel number */
	
   	sConfig6.Channel      = ADC_CHANNEL_11;                /* Sampled channel number */
	
 
   	sConfig5.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
   	sConfig5.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;    /* Sampling time (number of clock cycles unit) */
   	sConfig5.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
   	sConfig5.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */
   	sConfig5.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */ 
 
   	sConfig6.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
   	sConfig6.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;    /* Sampling time (number of clock cycles unit) */
   	sConfig6.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
   	sConfig6.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */
   	sConfig6.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */ 
   	
	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig5) != HAL_OK)
   	{
    		/* Channel Configuration Error */
   		printf( "Channel 10 Configuration Error\r\n");
		return 1;
   	}

	/*
   	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig6) != HAL_OK)
   	{
    		// Channel Configuration Error
   		printf( "Channel 11 Configuration Error\r\n");
		return 1;
   	}
	*/
 
   	/* Run the ADC calibration in single-ended mode */
   	if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
   	{
     		/* Calibration Error */
   		printf( "ADC Calibration Error\r\n");
   		return 1;
   	}
 
   	/*##-3- Start the conversion process #######################################*/
   	if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
   	{
     		/* Start Conversation Error */
   		printf( "Start Conversion Error\r\n");
   		return 1;
   	}
	return 0;
}

int dash_init(void) {

	/* dash specific initializations */
	if( adc_init() != 0 ){
		printf( "ADC5 Init Error.\r\n");
	}

	return 0;
}

int main(void) {

	PC_Buffer *rx;
	PC_Buffer *ctrl_rx;
     	uint16_t ADC5ConvertedValue;
     	uint16_t ADC6ConvertedValue;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init() || dash_init())
		fault();

	rx = get_rx(USB_UART);
	ctrl_rx = get_rx(USART1);

	post("Dashboard");
	printPrompt();

	while (1) {
		check_input(rx);
		check_incoming_controls(ctrl_rx);
		blink_handler(BLINK_INTERVAL);
		/*##-4- Wait for the end of conversion #####################################*/
		/*  For simplicity reasons, this example is just waiting till the end of the
       		conversion, but application may perform other tasks while conversion
       		operation is ongoing. */
   		if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK){
     			/* End Of Conversion flag not set on time */
   			printf("ADC End of conversion error.\r\n");
		}
   		else{
     			/* ADC conversion completed */
    	 		/*##-5- Get the converted value of regular channel  #########*/
     			ADC5ConvertedValue = HAL_ADC_GetValue(&AdcHandle);
   			printf("Our converted ADC5 Value: %x\r\n", ADC5ConvertedValue);
		}
	}

	return 0;
}
