#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "uart.h"
#include "dashboard_control.h"

#define BLINK_INTERVAL	250

/* ADC Info */
//PA5 is CURR_SENSE, and is connected to ADC5
//PA6 is VOLT_SENSE, and is connected to ADC6
ADC_HandleTypeDef    Adc5Handle;
ADC_HandleTypeDef    Adc6Handle;

/* Nucleo 32 I/O */
//THERM1: between batteries 1 and 2
//THERM2: between batteries 2 and 3
FILL_GPIO(THERM1,     GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(THERM2,	  GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR)

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
FILL_GPIO(SD_CS,	 GPIOB, 0, OUTPUT, LOW_SPEED, NONE, true, OTHER )

inline void printPrompt(void) {
	fputs("[dash-build] $ ", stdout);
	fflush(stdout);
}

int adc_init(void){

	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	Adc5Handle.Instance          = ADC5;
	Adc6Handle.Instance          = ADC6;

   	if (HAL_ADC_DeInit(&Adc5Handle) != HAL_OK)
   	{
     		/* ADC de-initialization Error */
   		printf( "ADC De-Init Error\r\n");
   		return 1;
	}
   	if (HAL_ADC_DeInit(&Adc6Handle) != HAL_OK)
   	{
     		/* ADC de-initialization Error */
   		printf( "ADC De-Init Error\r\n");
   		return 1;
	}
 
   	Adc5Handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */ 
   	Adc5Handle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */ 
   	Adc5Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */ 
   	Adc5Handle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */ 
   	Adc5Handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */ 
   	Adc5Handle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */ 
   	Adc5Handle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */ 
   	Adc5Handle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	Adc5Handle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */ 
   	Adc5Handle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	Adc5Handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */ 
   	Adc5Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */ 
   	Adc5Handle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */ 
   	Adc5Handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */ 
   	Adc5Handle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */ 


   	Adc6Handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;          /* Asynchronous clock mode, input ADC clock not divided */ 
   	Adc6Handle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */ 
   	Adc6Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */ 
   	Adc6Handle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */ 
   	Adc6Handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */ 
   	Adc6Handle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */ 
   	Adc6Handle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */ 
   	Adc6Handle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	Adc6Handle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */ 
   	Adc6Handle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */ 
   	Adc6Handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */ 
   	Adc6Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */ 
   	Adc6Handle.Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */ 
   	Adc6Handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */ 
   	Adc6Handle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */ 


	if (HAL_ADC_Init(&Adc5Handle) != HAL_OK)
	{
     		/* ADC initialization Error */
   		printf( "ADC Initialization Error\r\n");
   		return 1;
   	}
	if (HAL_ADC_Init(&Adc6Handle) != HAL_OK)
	{
     		/* ADC initialization Error */
   		printf( "ADC Initialization Error\r\n");
   		return 1;
   	}
 
   	/*##-2- Configure ADC regular channel ######################################*/

   	if( acd_num == 5){
   		sConfig.Channel      = ADC5_CHANNEL;                /* Sampled channel number */
	}
	else{
   		sConfig.Channel      = ADC6_CHANNEL;                /* Sampled channel number */
	} 
   	sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
   	sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;    /* Sampling time (number of clock cycles unit) */
   	sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
   	sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */
   	sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */ 
 
   	if (HAL_ADC_ConfigChannel(&Adc5Handle, &sConfig) != HAL_OK)
   	{
    		/* Channel Configuration Error */
   		printf( "Channel Configuration Error\r\n");
		return 1;
   	}
   	if (HAL_ADC_ConfigChannel(&Adc6Handle, &sConfig) != HAL_OK)
   	{
    		/* Channel Configuration Error */
   		printf( "Channel Configuration Error\r\n");
		return 1;
   	}
 
   	/* Run the ADC calibration in single-ended mode */
   	if (HAL_ADCEx_Calibration_Start(&Adc5Handle, ADC_SINGLE_ENDED) != HAL_OK)
   	{
     		/* Calibration Error */
   		printf( "ADC Calibration Error\r\n");
   		return 1;
   	}
   	/* Run the ADC calibration in single-ended mode */
   	if (HAL_ADCEx_Calibration_Start(&Adc6Handle, ADC_SINGLE_ENDED) != HAL_OK)
   	{
     		/* Calibration Error */
   		printf( "ADC Calibration Error\r\n");
   		return 1;
   	}
 
   	/*##-3- Start the conversion process #######################################*/
   	if (HAL_ADC_Start(&Adc5Handle) != HAL_OK)
   	{
     		/* Start Conversation Error */
   		printf( "Start Conversion Error\r\n");
   		return 1;
   	}
   	/*##-3- Start the conversion process #######################################*/
   	if (HAL_ADC_Start(&Adc6Handle) != HAL_OK)
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
     			Error_Handler();
   		}
   		else{
     			/* ADC conversion completed */
    	 		/*##-5- Get the converted value of regular channel  #########*/
     			uhADC5ConvertedValue = HAL_ADC_GetValue(&AdcHandle);
   			printf("Our converted ADC5 Value: %x\r\n", uhADC5ConvertedValue);
		}
	}

	return 0;
}
