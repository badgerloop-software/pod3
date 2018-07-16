/**
  ******************************************************************************
  * @file    stm32l4xx_hal_adc.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC)
  *          peripheral:
  *           + Initialization and de-initialization functions
  *             ++ Initialization and Configuration of ADC
  *           + Operation functions
  *             ++ Start, stop, get result of conversions of regular
  *                group, using 3 possible modes: polling, interruption or DMA.
  *           + Control functions
  *             ++ Channels configuration on regular group
  *             ++ Analog Watchdog configuration
  *           + State functions
  *             ++ ADC state machine management
  *             ++ Interrupts and flags management
  *          Other functions (extended functions) are available in file 
  *          "stm32l4xx_hal_adc_ex.c".
  *
  @verbatim
  ==============================================================================
                     ##### ADC peripheral features #####
  ==============================================================================
  [..]
  (+) 12-bit, 10-bit, 8-bit or 6-bit configurable resolution.

  (+) Interrupt generation at the end of regular conversion and in case of 
      analog watchdog or overrun events.
  
  (+) Single and continuous conversion modes.
  
  (+) Scan mode for conversion of several channels sequentially.
  
  (+) Data alignment with in-built data coherency.
  
  (+) Programmable sampling time (channel wise)
  
  (+) External trigger (timer or EXTI) with configurable polarity
  
  (+) DMA request generation for transfer of conversions data of regular group.
  
  (+) Configurable delay between conversions in Dual interleaved mode.
  
  (+) ADC channels selectable single/differential input.
  
  (+) ADC offset shared on 4 offset instances.
  (+) ADC calibration
  
  (+) ADC conversion of regular group.
  
  (+) ADC supply requirements: 1.62 V to 3.6 V.
  
  (+) ADC input range: from Vref- (connected to Vssa) to Vref+ (connected to 
      Vdda or to an external voltage reference).


                     ##### How to use this driver #####
  ==============================================================================
    [..]

     *** Configuration of top level parameters related to ADC ***
     ============================================================
     [..]

    (#) Enable the ADC interface
        (++) As prerequisite, ADC clock must be configured at RCC top level.

        (++) Two clock settings are mandatory: 
             (+++) ADC clock (core clock, also possibly conversion clock).

             (+++) ADC clock (conversions clock).
                   Two possible clock sources: synchronous clock derived from APB clock
                   or asynchronous clock derived from system clock, PLLSAI1 or the PLLSAI2 
                   running up to 80MHz.

             (+++) Example:
                   Into HAL_ADC_MspInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) __HAL_RCC_ADC_CLK_ENABLE();                  (mandatory)

               RCC_ADCCLKSOURCE_PLL enable:                       (optional: if asynchronous clock selected)
               (+++) RCC_PeriphClkInitTypeDef   RCC_PeriphClkInit;
               (+++) PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
               (+++) PeriphClkInit.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL;
               (+++) HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

        (++) ADC clock source and clock prescaler are configured at ADC level with
             parameter "ClockPrescaler" using function HAL_ADC_Init().

    (#) ADC pins configuration
         (++) Enable the clock for the ADC GPIOs
              using macro __HAL_RCC_GPIOx_CLK_ENABLE()
         (++) Configure these ADC pins in analog mode
              using function HAL_GPIO_Init()

    (#) Optionally, in case of usage of ADC with interruptions:
         (++) Configure the NVIC for ADC
              using function HAL_NVIC_EnableIRQ(ADCx_IRQn)
         (++) Insert the ADC interruption handler function HAL_ADC_IRQHandler() 
              into the function of corresponding ADC interruption vector 
              ADCx_IRQHandler().

    (#) Optionally, in case of usage of DMA:
         (++) Configure the DMA (DMA channel, mode normal or circular, ...)
              using function HAL_DMA_Init().
         (++) Configure the NVIC for DMA
              using function HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)
         (++) Insert the ADC interruption handler function HAL_ADC_IRQHandler() 
              into the function of corresponding DMA interruption vector 
              DMAx_Channelx_IRQHandler().

     *** Configuration of ADC, group regular, channels parameters ***
     ================================================================
     [..]

    (#) Configure the ADC parameters (resolution, data alignment, ...)
        and regular group parameters (conversion trigger, sequencer, ...)
        using function HAL_ADC_Init().

    (#) Configure the channels for regular group parameters (channel number, 
        channel rank into sequencer, ..., into regular group)
        using function HAL_ADC_ConfigChannel().

    (#) Optionally, configure the analog watchdog parameters (channels
        monitored, thresholds, ...)
        using function HAL_ADC_AnalogWDGConfig().

     *** Execution of ADC conversions ***
     ====================================
     [..]

    (#) Optionally, perform an automatic ADC calibration to improve the
        conversion accuracy
        using function HAL_ADCEx_Calibration_Start().

    (#) ADC driver can be used among three modes: polling, interruption,
        transfer by DMA.

        (++) ADC conversion by polling:
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start()
          (+++) Wait for ADC conversion completion 
                using function HAL_ADC_PollForConversion()
          (+++) Retrieve conversion results 
                using function HAL_ADC_GetValue()
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop()

        (++) ADC conversion by interruption: 
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start_IT()
          (+++) Wait for ADC conversion completion by call of function
                HAL_ADC_ConvCpltCallback()
                (this function must be implemented in user program)
          (+++) Retrieve conversion results 
                using function HAL_ADC_GetValue()
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop_IT()

        (++) ADC conversion with transfer by DMA:
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start_DMA()
          (+++) Wait for ADC conversion completion by call of function
                HAL_ADC_ConvCpltCallback() or HAL_ADC_ConvHalfCpltCallback()
                (these functions must be implemented in user program)
          (+++) Conversion results are automatically transferred by DMA into
                destination variable address.
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop_DMA()

     [..]

    (@) Callback functions must be implemented in user program:
      (+@) HAL_ADC_ErrorCallback()
      (+@) HAL_ADC_LevelOutOfWindowCallback() (callback of analog watchdog)
      (+@) HAL_ADC_ConvCpltCallback()
      (+@) HAL_ADC_ConvHalfCpltCallback

     *** Deinitialization of ADC ***
     ============================================================
     [..]

    (#) Disable the ADC interface
      (++) ADC clock can be hard reset and disabled at RCC top level.
        (++) Hard reset of ADC peripherals
             using macro __ADCx_FORCE_RESET(), __ADCx_RELEASE_RESET().
        (++) ADC clock disable
             using the equivalent macro/functions as configuration step.
             (+++) Example:
                   Into HAL_ADC_MspDeInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI14;
               (+++) RCC_OscInitStructure.HSI14State = RCC_HSI14_OFF; (if not used for system clock)
               (+++) HAL_RCC_OscConfig(&RCC_OscInitStructure);

    (#) ADC pins configuration
         (++) Disable the clock for the ADC GPIOs
              using macro __HAL_RCC_GPIOx_CLK_DISABLE()

    (#) Optionally, in case of usage of ADC with interruptions:
         (++) Disable the NVIC for ADC
              using function HAL_NVIC_EnableIRQ(ADCx_IRQn)

    (#) Optionally, in case of usage of DMA:
         (++) Deinitialize the DMA
              using function HAL_DMA_Init().
         (++) Disable the NVIC for DMA
              using function HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)

    [..]
  
    @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_ll_adc.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC HAL module driver
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup ADC_Private_Constants ADC Private Constants
  * @{
  */

#define ADC_CFGR_FIELDS_1  ((uint32_t)(ADC_CFGR_RES    | ADC_CFGR_ALIGN   |\
                                       ADC_CFGR_CONT   | ADC_CFGR_OVRMOD  |\
                                       ADC_CFGR_DISCEN | ADC_CFGR_DISCNUM |\
                                       ADC_CFGR_EXTEN  | ADC_CFGR_EXTSEL))   /*!< ADC_CFGR fields of parameters that can be updated 
                                                                                  when no regular conversion is on-going */
  
/* Delay to wait before setting ADEN once ADCAL has been reset
   must be at least 4 ADC clock cycles.
   Assuming lowest ADC clock (140 KHz according to DS), this
   4 ADC clock cycles duration is equal to
   4 / 140,000 = 0.028 ms.
   ADC_ENABLE_TIMEOUT set to 2 is a margin large enough to ensure
   the 4 ADC clock cycles have elapsed while waiting for ADRDY
   to become 1 */        
  #define ADC_ENABLE_TIMEOUT              ((uint32_t)  2)    /*!< ADC enable time-out value  */
  #define ADC_DISABLE_TIMEOUT             ((uint32_t)  2)    /*!< ADC disable time-out value */
   
  
/* Timeout to wait for current conversion on going to be completed.           */
/* Timeout fixed to longest ADC conversion possible, for 1 channel:           */
/*   - maximum sampling time (640.5 adc_clk)                                  */
/*   - ADC resolution (Tsar 12 bits= 12.5 adc_clk)                            */
/*   - ADC clock with prescaler 256                                           */
/*     (from asynchronous clock, assuming clock frequency same as CPU for     */
/*      this calculation)                                                     */
/*   - ADC oversampling ratio 256                                             */
/*   Calculation: 653 * 256 * 256 = 42795008 CPU clock cycles max             */
/* Unit: cycles of CPU clock.                                                 */
#define ADC_CONVERSION_TIME_MAX_CPU_CYCLES ((uint32_t) 42795008)  /*!< ADC conversion completion time-out value */



/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */

/** @defgroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @brief    ADC Initialization and Configuration functions
  *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the ADC. 
      (+) De-initialize the ADC.
@endverbatim
  * @{
  */

/**
  * @brief  Initialize the ADC peripheral and regular group according to  
  *         parameters specified in structure "ADC_InitTypeDef".
  * @note   As prerequisite, ADC clock must be configured at RCC top level
  *         (refer to description of RCC configuration for ADC
  *         in header of this file).
  * @note   Possibility to update parameters on the fly:
  *         This function initializes the ADC MSP (HAL_ADC_MspInit()) only when
  *         coming from ADC state reset. Following calls to this function can
  *         be used to reconfigure some parameters of ADC_InitTypeDef  
  *         structure on the fly, without modifying MSP configuration. If ADC  
  *         MSP has to be modified again, HAL_ADC_DeInit() must be called
  *         before HAL_ADC_Init().
  *         The setting of these parameters is conditioned to ADC state.
  *         For parameters constraints, see comments of structure 
  *         "ADC_InitTypeDef".
  * @note   This function configures the ADC within 2 scopes: scope of entire 
  *         ADC and scope of regular group. For parameters details, see comments 
  *         of structure "ADC_InitTypeDef".
  * @note   Parameters related to common ADC registers (ADC clock mode) are set 
  *         only if all ADCs are disabled.
  *         If this is not the case, these common parameters setting are  
  *         bypassed without error reporting: it can be the intended behaviour in
  *         case of update of a parameter of ADC_InitTypeDef on the fly,
  *         without  disabling the other ADCs.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmpCFGR = 0U;
  __IO uint32_t wait_loop_index = 0;
  
  /* Check ADC handle */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CLOCKPRESCALER(hadc->Init.ClockPrescaler));
  assert_param(IS_ADC_RESOLUTION(hadc->Init.Resolution));
  assert_param(IS_ADC_DFSDMCFG_MODE(hadc));
  assert_param(IS_ADC_DATA_ALIGN(hadc->Init.DataAlign));
  assert_param(IS_ADC_SCAN_MODE(hadc->Init.ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXTTRIG_EDGE(hadc->Init.ExternalTrigConvEdge));
  assert_param(IS_ADC_EXTTRIG(hadc, hadc->Init.ExternalTrigConv));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DMAContinuousRequests));
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  assert_param(IS_ADC_OVERRUN(hadc->Init.Overrun));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.LowPowerAutoWait));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.OversamplingMode));
  
  if(hadc->Init.ScanConvMode != ADC_SCAN_DISABLE)
  {
    assert_param(IS_ADC_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));
    assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DiscontinuousConvMode));
    
    if(hadc->Init.DiscontinuousConvMode == ENABLE)
    {
      assert_param(IS_ADC_REGULAR_DISCONT_NUMBER(hadc->Init.NbrOfDiscConversion));
    }
  }
  
  /* DISCEN and CONT bits cannot be set at the same time */
  assert_param(!((hadc->Init.DiscontinuousConvMode == ENABLE) && (hadc->Init.ContinuousConvMode == ENABLE)));
  
  /* Actions performed only if ADC is coming from state reset:                */
  /* - Initialization of ADC MSP                                              */
  if(hadc->State == HAL_ADC_STATE_RESET)
  {
    /* Init the low level hardware */
    HAL_ADC_MspInit(hadc);
    
    /* Set ADC error code to none */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* Initialize Lock */
    hadc->Lock = HAL_UNLOCKED;
  }
  
  /* - Exit from deep-power-down mode and ADC voltage regulator enable        */
  if(LL_ADC_IsDeepPowerDownEnabled(hadc->Instance) != 0U)
  {
    /* Disable ADC deep power down mode */ 
    LL_ADC_DisableDeepPowerDown(hadc->Instance);
    
    /* System was in deep power down mode, calibration must
     be relaunched or a previously saved calibration factor 
     re-applied once the ADC voltage regulator is enabled */    
  }
  
  if(LL_ADC_IsInternalRegulatorEnabled(hadc->Instance) == 0U)
  {
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(hadc->Instance);
    
    /* Delay for ADC stabilization time */
    /* Wait loop initialization and execution */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles.                                           */
    wait_loop_index = (LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (1000000 * 2)));
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
  }
  
  /* Verification that ADC voltage regulator is correctly enabled, whether    */
  /* or not ADC is coming from state reset (if any potential problem of       */
  /* clocking, voltage regulator would not be enabled).                       */
  if(LL_ADC_IsInternalRegulatorEnabled(hadc->Instance) == 0U)
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
    
    /* Set ADC error code to ADC IP internal error */
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
    
    tmp_hal_status = HAL_ERROR;
  }
  
  /* Configuration of ADC parameters if previous preliminary actions are      */ 
  /* correctly completed and if there is no conversion on going on regular    */
  /* group (ADC may already be enabled at this point if HAL_ADC_Init() is     */
  /* called to update a parameter on the fly).                                */
  if(   (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
     && (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
    )
  {
    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);
    
    /* Configuration of common ADC parameters                                 */
    
    /* Parameters update conditioned to ADC state:                            */
    /* Parameters that can be updated only when ADC is disabled:              */
    /*  - clock configuration                                                 */
    if ((ADC_IS_ENABLE(hadc) == RESET)   &&
         (ADC_ANY_OTHER_ENABLED(hadc) == RESET) )
    {
      /* Reset configuration of ADC common register CCR:                      */
      /*                                                                      */
      /*   - ADC clock mode and ACC prescaler (CKMODE and PRESC bits)are set  */
      /*     according to adc->Init.ClockPrescaler. It selects the clock      */
      /*    source and sets the clock division factor.                        */
      /*                                                                      */
      /* Some parameters of this register are not reset, since they are set   */
      /* by other functions and must be kept in case of usage of this         */
      /* function on the fly (update of a parameter of ADC_InitTypeDef        */
      /* without needing to reconfigure all other ADC groups/channels         */
      /* parameters):                                                         */
      /*   - when multimode feature is available, multimode-related           */
      /*     parameters: MDMA, DMACFG, DELAY, DUAL (set by API                */
      /*     HAL_ADCEx_MultiModeConfigChannel() )                             */
      /*   - internal measurement paths: Vbat, temperature sensor, Vref       */
      /*     (set into HAL_ADC_ConfigChannel() or                             */
      /*     HAL_ADCEx_InjectedConfigChannel() )                              */
      LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(hadc->Instance), hadc->Init.ClockPrescaler);
    }
    
    /* Configuration of ADC:                                                  */
    /*  - resolution                               Init.Resolution            */
    /*  - data alignment                           Init.DataAlign             */
    /*  - external trigger to start conversion     Init.ExternalTrigConv      */
    /*  - external trigger polarity                Init.ExternalTrigConvEdge  */
    /*  - continuous conversion mode               Init.ContinuousConvMode    */
    /*  - overrun                                  Init.Overrun               */
    /*  - discontinuous mode                       Init.DiscontinuousConvMode */
    /*  - discontinuous mode channel count         Init.NbrOfDiscConversion   */
    tmpCFGR  = (ADC_CFGR_CONTINUOUS(hadc->Init.ContinuousConvMode)           |
                hadc->Init.Overrun                                           |
                hadc->Init.DataAlign                                         |
                hadc->Init.Resolution                                        |
                ADC_CFGR_REG_DISCONTINUOUS(hadc->Init.DiscontinuousConvMode)  );
    
    if (hadc->Init.DiscontinuousConvMode == ENABLE)
    {
      tmpCFGR |= ADC_CFGR_DISCONTINUOUS_NUM(hadc->Init.NbrOfDiscConversion);
    }
    
    /* Enable external trigger if trigger selection is different of software  */
    /* start.                                                                 */
    /* Note: This configuration keeps the hardware feature of parameter       */
    /*       ExternalTrigConvEdge "trigger edge none" equivalent to           */
    /*       software start.                                                  */
    if (hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
    {
      tmpCFGR |= (  (hadc->Init.ExternalTrigConv & ADC_CFGR_EXTSEL)
                  | hadc->Init.ExternalTrigConvEdge
                 );
    }
    
    /* Update Configuration Register CFGR */
    MODIFY_REG(hadc->Instance->CFGR, ADC_CFGR_FIELDS_1, tmpCFGR); 
    
    /* Parameters update conditioned to ADC state:                            */
    /* Parameters that can be updated when ADC is disabled or enabled without */
    /* conversion on going on regular and injected groups:                    */
    /*  - DMA continuous request          Init.DMAContinuousRequests          */
    /*  - LowPowerAutoWait feature        Init.LowPowerAutoWait               */
    /*  - Oversampling parameters         Init.Oversampling                   */
    if (ADC_IS_CONVERSION_ONGOING_REGULAR_INJECTED(hadc) == RESET)
    {
      tmpCFGR = ( ADC_CFGR_DFSDM(hadc)                                 |
                  ADC_CFGR_AUTOWAIT(hadc->Init.LowPowerAutoWait)       |
                  ADC_CFGR_DMACONTREQ(hadc->Init.DMAContinuousRequests) );
      
      MODIFY_REG(hadc->Instance->CFGR, ADC_CFGR_FIELDS_2, tmpCFGR);                    
      
      if (hadc->Init.OversamplingMode == ENABLE)
      {
        assert_param(IS_ADC_OVERSAMPLING_RATIO(hadc->Init.Oversampling.Ratio));
        assert_param(IS_ADC_RIGHT_BIT_SHIFT(hadc->Init.Oversampling.RightBitShift));
        assert_param(IS_ADC_TRIGGERED_OVERSAMPLING_MODE(hadc->Init.Oversampling.TriggeredMode));
        assert_param(IS_ADC_REGOVERSAMPLING_MODE(hadc->Init.Oversampling.OversamplingStopReset));
        
        /* Configuration of Oversampler:                                      */
        /*  - Oversampling Ratio                                              */
        /*  - Right bit shift                                                 */
        /*  - Triggered mode                                                  */
        /*  - Oversampling mode (continued/resumed)                           */
        MODIFY_REG(hadc->Instance->CFGR2,
                   ADC_CFGR2_OVSR  |
                   ADC_CFGR2_OVSS  |
                   ADC_CFGR2_TROVS |
                   ADC_CFGR2_ROVSM,
                   ADC_CFGR2_ROVSE                       |
                   hadc->Init.Oversampling.Ratio         |
                   hadc->Init.Oversampling.RightBitShift |
                   hadc->Init.Oversampling.TriggeredMode |
                   hadc->Init.Oversampling.OversamplingStopReset
                  );
      }
      else
      {
        /* Disable ADC oversampling scope on ADC group regular */
        CLEAR_BIT(hadc->Instance->CFGR2, ADC_CFGR2_ROVSE);
      }
      
    }   /*  if (ADC_IS_CONVERSION_ONGOING_REGULAR_INJECTED(hadc) == RESET) */
    
    /* Configuration of regular group sequencer:                              */
    /* - if scan mode is disabled, regular channels sequence length is set to */
    /*   0x00: 1 channel converted (channel on regular rank 1)                */
    /*   Parameter "NbrOfConversion" is discarded.                            */
    /*   Note: Scan mode is not present by hardware on this device, but       */
    /*   emulated by software for alignment over all STM32 devices.           */
    /* - if scan mode is enabled, regular channels sequence length is set to  */
    /*   parameter "NbrOfConversion".                                         */

    if (hadc->Init.ScanConvMode == ADC_SCAN_ENABLE)
    {
      /* Set number of ranks in regular group sequencer */
      MODIFY_REG(hadc->Instance->SQR1, ADC_SQR1_L, (hadc->Init.NbrOfConversion - (uint8_t)1));
    }
    else
    {
      CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_L);
    }
    
    /* Initialize the ADC state */
    /* Clear HAL_ADC_STATE_BUSY_INTERNAL bit, set HAL_ADC_STATE_READY bit */
    ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL, HAL_ADC_STATE_READY);
  }
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
    
    tmp_hal_status = HAL_ERROR;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Deinitialize the ADC peripheral registers to their default reset
  *         values, with deinitialization of the ADC MSP.
  * @note   For devices with several ADCs: reset of ADC common registers is done 
  *         only if all ADCs sharing the same common group are disabled.
  *         (function "HAL_ADC_MspDeInit()" is also called under the same conditions:
  *         all ADC instances use the same core clock at RCC level, disabling
  *         the core clock reset all ADC instances).
  *         If this is not the case, reset of these common parameters reset is  
  *         bypassed without error reporting: it can be the intended behavior in
  *         case of reset of a single ADC while the other ADCs sharing the same 
  *         common group is still running.
  * @note   By default, HAL_ADC_DeInit() set ADC in mode deep power-down:
  *         this saves more power by reducing leakage currents 
  *         and is particularly interesting before entering MCU low-power modes.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc)
{
  /* Check ADC handle */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Set ADC state */
  SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);
  
  /* Stop potential conversion on going, on regular and injected groups */
  /* Note: No check on ADC_ConversionStop() return status,              */
  /*       if the conversion stop failed, it is up to                   */
  /*       HAL_ADC_MspDeInit() to reset the ADC IP.                     */
  ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);
  
  /* Disable ADC peripheral if conversions are effectively stopped            */
  /* Flush register JSQR: reset the queue sequencer when injected             */
  /* queue sequencer is enabled and ADC disabled.                             */
  /* The software and hardware triggers of the injected sequence are both     */
  /* internally disabled just after the completion of the last valid          */
  /* injected sequence.                                                       */
  SET_BIT(hadc->Instance->CFGR, ADC_CFGR_JQM);
  
  /* Disable the ADC peripheral */
  /* No check on ADC_Disable() return status, if the ADC disabling process
    failed, it is up to HAL_ADC_MspDeInit() to reset the ADC IP */  
  ADC_Disable(hadc);
  
  
  /* ========== Reset ADC registers ========== */
  /* Reset register IER */
  __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_AWD3  | ADC_IT_AWD2 | ADC_IT_AWD1 |
                              ADC_IT_JQOVF | ADC_IT_OVR  |
                              ADC_IT_JEOS  | ADC_IT_JEOC |
                              ADC_IT_EOS   | ADC_IT_EOC  |
                              ADC_IT_EOSMP | ADC_IT_RDY                 ) );
      
  /* Reset register ISR */
  __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_AWD3  | ADC_FLAG_AWD2 | ADC_FLAG_AWD1 |
                              ADC_FLAG_JQOVF | ADC_FLAG_OVR  |
                              ADC_FLAG_JEOS  | ADC_FLAG_JEOC |
                              ADC_FLAG_EOS   | ADC_FLAG_EOC  |
                              ADC_FLAG_EOSMP | ADC_FLAG_RDY                   ) );
  
  /* Reset register CR */
 /* Bits ADC_CR_JADSTP, ADC_CR_ADSTP, ADC_CR_JADSTART, ADC_CR_ADSTART, 
    ADC_CR_ADCAL, ADC_CR_ADDIS and ADC_CR_ADEN are in access mode "read-set": 
    no direct reset applicable. 
    Update CR register to reset value where doable by software */
  CLEAR_BIT(hadc->Instance->CR, ADC_CR_ADVREGEN | ADC_CR_ADCALDIF);
  SET_BIT(hadc->Instance->CR, ADC_CR_DEEPPWD);    
      
  /* Reset register CFGR */
  CLEAR_BIT(hadc->Instance->CFGR, ADC_CFGR_FIELDS);
  SET_BIT(hadc->Instance->CFGR, ADC_CFGR_JQDIS);  
  
  /* Reset register CFGR2 */
  CLEAR_BIT(hadc->Instance->CFGR2, ADC_CFGR2_ROVSM  | ADC_CFGR2_TROVS   | ADC_CFGR2_OVSS |   
                                  ADC_CFGR2_OVSR  | ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE    );                                 
  
  /* Reset register SMPR1 */
  CLEAR_BIT(hadc->Instance->SMPR1, ADC_SMPR1_FIELDS);                                 
  
  /* Reset register SMPR2 */
  CLEAR_BIT(hadc->Instance->SMPR2, ADC_SMPR2_SMP18 | ADC_SMPR2_SMP17 | ADC_SMPR2_SMP16 | 
                             ADC_SMPR2_SMP15 | ADC_SMPR2_SMP14 | ADC_SMPR2_SMP13 | 
                             ADC_SMPR2_SMP12 | ADC_SMPR2_SMP11 | ADC_SMPR2_SMP10    );                                 
  
  /* Reset register TR1 */
  CLEAR_BIT(hadc->Instance->TR1, ADC_TR1_HT1 | ADC_TR1_LT1);
  
  /* Reset register TR2 */
  CLEAR_BIT(hadc->Instance->TR2, ADC_TR2_HT2 | ADC_TR2_LT2);    
  
  /* Reset register TR3 */
  CLEAR_BIT(hadc->Instance->TR3, ADC_TR3_HT3 | ADC_TR3_LT3);      
  
  /* Reset register SQR1 */
  CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_SQ4 | ADC_SQR1_SQ3 | ADC_SQR1_SQ2 | 
                            ADC_SQR1_SQ1 | ADC_SQR1_L);                              
  
  /* Reset register SQR2 */
  CLEAR_BIT(hadc->Instance->SQR2, ADC_SQR2_SQ9 | ADC_SQR2_SQ8 | ADC_SQR2_SQ7 | 
                            ADC_SQR2_SQ6 | ADC_SQR2_SQ5);                                
  
  /* Reset register SQR3 */
  CLEAR_BIT(hadc->Instance->SQR3, ADC_SQR3_SQ14 | ADC_SQR3_SQ13 | ADC_SQR3_SQ12 | 
                            ADC_SQR3_SQ11 | ADC_SQR3_SQ10);                                 
  
  /* Reset register SQR4 */
  CLEAR_BIT(hadc->Instance->SQR4, ADC_SQR4_SQ16 | ADC_SQR4_SQ15); 
  
  /* Register JSQR was reset when the ADC was disabled */
  
  /* Reset register DR */
  /* bits in access mode read only, no direct reset applicable*/
    
  /* Reset register OFR1 */
  CLEAR_BIT(hadc->Instance->OFR1, ADC_OFR1_OFFSET1_EN | ADC_OFR1_OFFSET1_CH | ADC_OFR1_OFFSET1); 
  /* Reset register OFR2 */
  CLEAR_BIT(hadc->Instance->OFR2, ADC_OFR2_OFFSET2_EN | ADC_OFR2_OFFSET2_CH | ADC_OFR2_OFFSET2); 
  /* Reset register OFR3 */
  CLEAR_BIT(hadc->Instance->OFR3, ADC_OFR3_OFFSET3_EN | ADC_OFR3_OFFSET3_CH | ADC_OFR3_OFFSET3); 
  /* Reset register OFR4 */
  CLEAR_BIT(hadc->Instance->OFR4, ADC_OFR4_OFFSET4_EN | ADC_OFR4_OFFSET4_CH | ADC_OFR4_OFFSET4);
  
  /* Reset registers JDR1, JDR2, JDR3, JDR4 */
  /* bits in access mode read only, no direct reset applicable*/
  
  /* Reset register AWD2CR */
  CLEAR_BIT(hadc->Instance->AWD2CR, ADC_AWD2CR_AWD2CH);
  
  /* Reset register AWD3CR */
  CLEAR_BIT(hadc->Instance->AWD3CR, ADC_AWD3CR_AWD3CH);
  
  /* Reset register DIFSEL */
  CLEAR_BIT(hadc->Instance->DIFSEL, ADC_DIFSEL_DIFSEL);
  
  /* Reset register CALFACT */
  CLEAR_BIT(hadc->Instance->CALFACT, ADC_CALFACT_CALFACT_D | ADC_CALFACT_CALFACT_S);
  
  
  /* ========== Reset common ADC registers ========== */
        
  /* Software is allowed to change common parameters only when all the other
     ADCs are disabled.   */
  if ((ADC_IS_ENABLE(hadc) == RESET)   &&
       (ADC_ANY_OTHER_ENABLED(hadc) == RESET) )
  {
    /* Reset configuration of ADC common register CCR:
      - clock mode: CKMODE, PRESCEN
      - multimode related parameters (when this feature is available): MDMA, 
        DMACFG, DELAY, DUAL (set by HAL_ADCEx_MultiModeConfigChannel() API)
      - internal measurement paths: Vbat, temperature sensor, Vref (set into
        HAL_ADC_ConfigChannel() or HAL_ADCEx_InjectedConfigChannel() )
    */
    ADC_CLEAR_COMMON_CONTROL_REGISTER(hadc);
  }

  /* DeInit the low level hardware. 
  
     For example:
    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();
    __HAL_RCC_ADC_CLK_DISABLE();
    
    Keep in mind that all ADCs use the same clock: disabling
    the clock will reset all ADCs. 
    
  */
  HAL_ADC_MspDeInit(hadc);
    
  /* Set ADC error code to none */
  ADC_CLEAR_ERRORCODE(hadc);
  
  /* Reset injected channel configuration parameters */
  hadc->InjectionConfig.ContextQueue = 0;
  hadc->InjectionConfig.ChannelCount = 0; 
    
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_RESET;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initialize the ADC MSP.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspInit must be implemented in the user file.
   */ 
}

/**
  * @brief  DeInitialize the ADC MSP.
  * @param hadc ADC handle
  * @note   All ADC instances use the same core clock at RCC level, disabling
  *         the core clock reset all ADC instances).
  * @retval None
  */
__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspDeInit must be implemented in the user file.
   */ 
}

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group2 ADC Input and Output operation functions
 *  @brief    ADC IO operation functions 
 *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion of regular group.
      (+) Stop conversion of regular group.
      (+) Poll for conversion complete on regular group.
      (+) Poll for conversion event.
      (+) Get result of regular channel conversion.
      (+) Start conversion of regular group and enable interruptions.
      (+) Stop conversion of regular group and disable interruptions.
      (+) Handle ADC interrupt request
      (+) Start conversion of regular group and enable DMA transfer.
      (+) Stop conversion of regular group and disable ADC DMA transfer.
@endverbatim
  * @{
  */

/**
  * @brief  Enable ADC, start conversion of regular group.
  * @note   Interruptions enabled in this function: None.
  * @note   Case of multimode enabled (when multimode feature is available): 
  *           if ADC is Slave, ADC is enabled but conversion is not started, 
  *           if ADC is master, ADC is enabled and multimode conversion is started.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  ADC_TypeDef       *tmpADC_Master;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);
    
    /* Enable the ADC peripheral */
    tmp_hal_status = ADC_Enable(hadc);
    
    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
      
      /* Reset HAL_ADC_STATE_MULTIMODE_SLAVE bit
        - by default if ADC is Master or Independent or if multimode feature is not available
        - if multimode setting is set to independent mode (no dual regular or injected conversions are configured) */
      if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
      {
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      }

      /* Set ADC error code */
      /* Check if a conversion is on going on ADC group injected */
      if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        /* Reset ADC error code fields related to regular conversions only */
        CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
      }
      else
      {  
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc); 
      }
      
      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
      
      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);
      
      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Case of multimode enabled (when multimode feature is available):     */
      /*  - if ADC is slave and dual regular conversions are enabled, ADC is  */
      /*    enabled only (conversion is not started),                         */
      /*  - if ADC is master, ADC is enabled and conversion is started.       */
      if (ADC_INDEPENDENT_OR_NONMULTIMODEREGULAR_SLAVE(hadc))
      {
        /* Multimode feature is not available or ADC Instance is Independent or Master, 
           or is not Slave ADC with dual regular conversions enabled. 
           Then, set HAL_ADC_STATE_INJ_BUSY bit and reset HAL_ADC_STATE_INJ_EOC bit if JAUTO is set. */
        if (READ_BIT(hadc->Instance->CFGR, ADC_CFGR_JAUTO) != RESET)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
        }
        
        /* Start ADC group regular conversion */
        LL_ADC_REG_StartConversion(hadc->Instance);
      }
      else
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
        /* if Master ADC JAUTO bit is set, update Slave State in setting 
           HAL_ADC_STATE_INJ_BUSY bit and in resetting HAL_ADC_STATE_INJ_EOC bit */
        tmpADC_Master = ADC_MASTER_REGISTER(hadc); 
        if (READ_BIT(tmpADC_Master->CFGR, ADC_CFGR_JAUTO) != RESET)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
        }
        
      }
    }
    else
    {
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected channels in 
  *         case of auto_injection mode), disable ADC peripheral.
  * @note:  ADC peripheral disable is forcing stop of potential 
  *         conversion on injected group. If injected group is under use, it
  *         should be preliminarily stopped using HAL_ADCEx_InjectedStop function.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential conversion on going, on ADC groups regular and injected */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* 2. Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);
    
    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Wait for regular group conversion to be completed.
  * @note   ADC conversion flags EOS (end of sequence) and EOC (end of
  *         conversion) are cleared by this function, with an exception:
  *         if low power feature "LowPowerAutoWait" is enabled, flags are 
  *         not cleared to not interfere with this feature until data register
  *         is read using function HAL_ADC_GetValue().
  * @note   This function cannot be used in a particular setup: ADC configured 
  *         in DMA mode and polling for end of each conversion (ADC init
  *         parameter "EOCSelection" set to ADC_EOC_SINGLE_CONV).
  *         In this case, DMA resets the flag EOC and polling cannot be
  *         performed on each conversion. Nevertheless, polling can still 
  *         be performed on the complete sequence (ADC init
  *         parameter "EOCSelection" set to ADC_EOC_SEQ_CONV).
  * @param hadc ADC handle
  * @param Timeout Timeout value in millisecond.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  uint32_t tmp_Flag_End = 0U;
  uint32_t tmp_cfgr = 0U;
  ADC_TypeDef *tmpADC_Master;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* If end of conversion selected to end of sequence conversions */
  if (hadc->Init.EOCSelection == ADC_EOC_SEQ_CONV)
  {
    tmp_Flag_End = ADC_FLAG_EOS;
  }
  /* If end of conversion selected to end of unitary conversion */
  else /* ADC_EOC_SINGLE_CONV */
  {
    /* Verification that ADC configuration is compliant with polling for      */
    /* each conversion:                                                       */
    /* Particular case is ADC configured in DMA mode and ADC sequencer with   */
    /* several ranks and polling for end of each conversion.                  */
    /* For code simplicity sake, this particular case is generalized to       */
    /* ADC configured in DMA mode and and polling for end of each conversion. */
    if(ADC_IS_DUAL_REGULAR_CONVERSION_ENABLE(hadc) == RESET)
    {
      /* Check ADC DMA mode in independant mode */
      if(READ_BIT(hadc->Instance->CFGR, ADC_CFGR_DMAEN) != RESET)
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        return HAL_ERROR;
      }
      else
      {
        tmp_Flag_End = (ADC_FLAG_EOC);
      }
    }
    else
    {
      /* Check ADC DMA mode in multimode */
      if(ADC_MULTIMODE_DMA_ENABLED(hadc))
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        return HAL_ERROR;
      }
      else
      {
        tmp_Flag_End = (ADC_FLAG_EOC);
      }
    }
  }
  
  /* Get tick count */
  tickstart = HAL_GetTick();
  
  /* Wait until End of unitary conversion or sequence conversions flag is raised */
  while(HAL_IS_BIT_CLR(hadc->Instance->ISR, tmp_Flag_End))
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0) || ((HAL_GetTick()-tickstart) > Timeout))
      {
        /* Update ADC state machine to timeout */
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
        
        return HAL_TIMEOUT;
      }
    }
  }
  
  /* Update ADC state machine */
  SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
  
  /* Determine whether any further conversion upcoming on group regular       */
  /* by external trigger, continuous mode or scan sequence on going.          */
  if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
     (hadc->Init.ContinuousConvMode == DISABLE)   )
  {
    /* Check whether end of sequence is reached */
    if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) )
    {
      /* Set ADC state */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);
      
      if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      { 
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
      }
    }
  }
  
  /* Get relevant register CFGR in ADC instance of ADC master or slave        */
  /* in function of multimode state (for devices with multimode               */
  /* available).                                                              */
  if (ADC_INDEPENDENT_OR_NONMULTIMODEREGULAR_SLAVE(hadc)) 
  {
    /* Retrieve handle ADC CFGR register */
    tmp_cfgr = READ_REG(hadc->Instance->CFGR);  
  }
  else
  {
    /* Retrieve Master ADC CFGR register */
    tmpADC_Master = ADC_MASTER_REGISTER(hadc);
    tmp_cfgr = READ_REG(tmpADC_Master->CFGR);
  }
  
  /* Clear polled flag */
  if (tmp_Flag_End == ADC_FLAG_EOS)
  {
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOS);  
  }
  else
  {
    /* Clear end of conversion EOC flag of regular group if low power feature */
    /* "LowPowerAutoWait " is disabled, to not interfere with this feature    */
    /* until data register is read using function HAL_ADC_GetValue().         */
    if (READ_BIT(tmp_cfgr, ADC_CFGR_AUTDLY) == RESET)
    {
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS));
    }
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Poll for ADC event.
  * @param hadc ADC handle
  * @param EventType the ADC event type.
  *          This parameter can be one of the following values:
  *            @arg @ref ADC_EOSMP_EVENT  ADC End of Sampling event
  *            @arg @ref ADC_AWD1_EVENT   ADC Analog watchdog 1 event (main analog watchdog, present on all STM32 devices)
  *            @arg @ref ADC_AWD2_EVENT   ADC Analog watchdog 2 event (additional analog watchdog, not present on all STM32 families)
  *            @arg @ref ADC_AWD3_EVENT   ADC Analog watchdog 3 event (additional analog watchdog, not present on all STM32 families)
  *            @arg @ref ADC_OVR_EVENT    ADC Overrun event
  *            @arg @ref ADC_JQOVF_EVENT  ADC Injected context queue overflow event
  * @param Timeout Timeout value in millisecond.
  * @note   The relevant flag is cleared if found to be set, except for ADC_FLAG_OVR.
  *         Indeed, the latter is reset only if hadc->Init.Overrun field is set  
  *         to ADC_OVR_DATA_OVERWRITTEN. Otherwise, data register may be potentially overwritten 
  *         by a new converted data as soon as OVR is cleared.
  *         To reset OVR flag once the preserved data is retrieved, the user can resort
  *         to macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR); 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout)
{
  uint32_t tickstart = 0; 
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EVENT_TYPE(EventType));
  
  /* Get tick count */
  tickstart = HAL_GetTick();
  
  /* Check selected event flag */
  while(__HAL_ADC_GET_FLAG(hadc, EventType) == RESET)
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        /* Update ADC state machine to timeout */
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
        
        return HAL_TIMEOUT;
      }
    }
  }
  
  switch(EventType)
  {
  /* End Of Sampling event */
  case ADC_EOSMP_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOSMP);
     
    /* Clear the End Of Sampling flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP);
       
    break;  
  
  /* Analog watchdog (level out of window) event */
  /* Note: In case of several analog watchdog enabled, if needed to know      */
  /* which one triggered and on which ADCx, test ADC state of analog watchdog */
  /* flags HAL_ADC_STATE_AWD1/2/3 using function "HAL_ADC_GetState()".        */
  /* For example:                                                             */
  /*  " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD1)) "    */
  /*  " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD2)) "    */
  /*  " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD3)) "    */
  /* Check analog watchdog 1 flag */
  case ADC_AWD_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    
    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD1);
    
    break;
  
  /* Check analog watchdog 2 flag */
  case ADC_AWD2_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD2);
      
    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD2);
    
    break;
  
  /* Check analog watchdog 3 flag */
  case ADC_AWD3_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD3);
      
    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD3);
    
    break;
  
  /* Injected context queue overflow event */
  case ADC_JQOVF_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_INJ_JQOVF);
    
    /* Set ADC error code to Injected context queue overflow */
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_JQOVF);
    
    /* Clear ADC Injected context queue overflow flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JQOVF);
    
    break;
    
  /* Overrun event */
  default: /* Case ADC_OVR_EVENT */
    /* If overrun is set to overwrite previous data, overrun event is not     */
    /* considered as an error.                                                */
    /* (cf ref manual "Managing conversions without using the DMA and without */
    /* overrun ")                                                             */
    if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_OVR);
        
      /* Set ADC error code to overrun */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);
    }
    else
    {
      /* Clear ADC Overrun flag only if Overrun is set to ADC_OVR_DATA_OVERWRITTEN
         otherwise, data register is potentially overwritten by new converted data as soon
         as OVR is cleared. */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
    }
    break;
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enable ADC, start conversion of regular group with interruption.
  * @note   Interruptions enabled in this function according to initialization
  *         setting : EOC (end of conversion), EOS (end of sequence), 
  *         OVR overrun.
  *         Each of these interruptions has its dedicated callback function.
  * @note   Case of multimode enabled (when multimode feature is available): 
  *         HAL_ADC_Start_IT() must be called for ADC Slave first, then for
  *         ADC Master.
  *         For ADC Slave, ADC is enabled only (conversion is not started).  
  *         For ADC Master, ADC is enabled and multimode conversion is started.
  * @note   To guarantee a proper reset of all interruptions once all the needed
  *         conversions are obtained, HAL_ADC_Stop_IT() must be called to ensure 
  *         a correct stop of the IT-based conversions.
  * @note   By default, HAL_ADC_Start_IT() does not enable the End Of Sampling 
  *         interruption. If required (e.g. in case of oversampling with trigger
  *         mode), the user must:
  *          1. first clear the EOSMP flag if set with macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP)             
  *          2. then enable the EOSMP interrupt with macro __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOSMP)     
  *          before calling HAL_ADC_Start_IT().
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  ADC_TypeDef       *tmpADC_Master;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);
    
    /* Enable the ADC peripheral */
    tmp_hal_status = ADC_Enable(hadc);
    
    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
      
      /* Reset HAL_ADC_STATE_MULTIMODE_SLAVE bit
        - by default if ADC is Master or Independent or if multimode feature is not available
        - if multimode setting is set to independent mode (no dual regular or injected conversions are configured) */
      if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
      {
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
      }
      
      /* Set ADC error code */
      /* Check if a conversion is on going on ADC group injected */
      if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
      {
        /* Reset ADC error code fields related to regular conversions only */
        CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR|HAL_ADC_ERROR_DMA));         
      }
      else
      {
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc); 
      }
      
      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
      
      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);
      
      /* Disable all interruptions before enabling the desired ones */
      __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR));
      
      /* Enable ADC end of conversion interrupt */
      switch(hadc->Init.EOCSelection)
      {
        case ADC_EOC_SEQ_CONV:
          __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOS);
          break;
        /* case ADC_EOC_SINGLE_CONV */
        default:
          __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);
          break;
      }
      
      /* Enable ADC overrun interrupt */
      /* If hadc->Init.Overrun is set to ADC_OVR_DATA_PRESERVED, only then is
         ADC_IT_OVR enabled; otherwise data overwrite is considered as normal
         behavior and no CPU time is lost for a non-processed interruption */
      if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
      {
        __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);  
      }
      
      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Case of multimode enabled (when multimode feature is available):     */ 
      /*  - if ADC is slave and dual regular conversions are enabled, ADC is  */
      /*    enabled only (conversion is not started),                         */
      /*  - if ADC is master, ADC is enabled and conversion is started.       */
      if(ADC_INDEPENDENT_OR_NONMULTIMODEREGULAR_SLAVE(hadc))
      {
        /* Multimode feature is not available or ADC Instance is Independent or Master, 
           or is not Slave ADC with dual regular conversions enabled.         
           Then set HAL_ADC_STATE_INJ_BUSY and reset HAL_ADC_STATE_INJ_EOC if JAUTO is set. */
        if (READ_BIT(hadc->Instance->CFGR, ADC_CFGR_JAUTO) != RESET)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
          
          /* Enable as well injected interruptions in case 
           HAL_ADCEx_InjectedStart_IT() has not been called beforehand. This
           allows to start regular and injected conversions when JAUTO is
           set with a single call to HAL_ADC_Start_IT() */
          switch(hadc->Init.EOCSelection)
          {
            case ADC_EOC_SEQ_CONV: 
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
              __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOS);
            break;
            /* case ADC_EOC_SINGLE_CONV */
            default:
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOS);      
              __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOC);
            break;
          }
        }
        
        /* Start ADC group regular conversion */
        LL_ADC_REG_StartConversion(hadc->Instance);
      }
      else
      {
        /* hadc is the handle of a Slave ADC with dual regular conversions
           enabled. Therefore, ADC_CR_ADSTART is NOT set */
        SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
        /* if Master ADC JAUTO bit is set, Slave injected interruptions
           are enabled nevertheless (for same reason as above) */
        tmpADC_Master = ADC_MASTER_REGISTER(hadc); 
        if (READ_BIT(tmpADC_Master->CFGR, ADC_CFGR_JAUTO) != RESET)
        {
          /* First, update Slave State in setting HAL_ADC_STATE_INJ_BUSY bit 
             and in resetting HAL_ADC_STATE_INJ_EOC bit */
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
          /* Next, set Slave injected interruptions */
          switch(hadc->Init.EOCSelection)
          {
            case ADC_EOC_SEQ_CONV:
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
              __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOS);
            break;
            /* case ADC_EOC_SINGLE_CONV */
            default:
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOS);
              __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOC);
            break;
          }
        }
      }
    }
    else
    {
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
    }
    
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected group in 
  *         case of auto_injection mode), disable interrution of 
  *         end-of-conversion, disable ADC peripheral.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential conversion on going, on ADC groups regular and injected */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* Disable ADC end of conversion interrupt for regular group */
    /* Disable ADC overrun interrupt */
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR));
    
    /* 2. Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);
    
    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
<<<<<<< 168a21ba563d7ffc06b8e983e70fa8069e873d01
<<<<<<< 7712d42dcb8046c308b95df4a3368f8b7ef96872
=======
  * @brief  Enable ADC, start conversion of regular group and transfer result through DMA.
  * @note   Interruptions enabled in this function:
  *         overrun (if applicable), DMA half transfer, DMA transfer complete. 
  *         Each of these interruptions has its dedicated callback function.
  * @note   Case of multimode enabled (when multimode feature is available): HAL_ADC_Start_DMA() 
  *         is designed for single-ADC mode only. For multimode, the dedicated 
  *         HAL_ADCEx_MultiModeStart_DMA() function must be used.
  * @param hadc ADC handle
  * @param pData Destination Buffer address.
  * @param Length Number of data to be transferred from ADC peripheral to memory
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);
    
    /* Ensure that dual regular conversions are not enabled or unavailable.   */
    /* Otherwise, dedicated API HAL_ADCEx_MultiModeStart_DMA() must be used.  */
    if (ADC_IS_DUAL_REGULAR_CONVERSION_ENABLE(hadc) == RESET)
    {
      /* Enable the ADC peripheral */
      tmp_hal_status = ADC_Enable(hadc);
    
      /* Start conversion if ADC is effectively enabled */
      if (tmp_hal_status == HAL_OK)
      {
        /* Set ADC state                                                        */
        /* - Clear state bitfield related to regular group conversion results   */
        /* - Set state bitfield related to regular operation                    */
        ADC_STATE_CLR_SET(hadc->State,
                          HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                          HAL_ADC_STATE_REG_BUSY);
        
        /* Reset HAL_ADC_STATE_MULTIMODE_SLAVE bit
          - by default if ADC is Master or Independent or if multimode feature is not available
          - if multimode setting is set to independent mode (no dual regular or injected conversions are configured) */
        if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
        {
          CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);
        }
        
        /* Check if a conversion is on going on ADC group injected */
        if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
        {
          /* Reset ADC error code fields related to regular conversions only */
          CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
        }
        else
        {  
          /* Reset all ADC error code fields */
          ADC_CLEAR_ERRORCODE(hadc); 
        }
        
        /* Set the DMA transfer complete callback */
        hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;
        
        /* Set the DMA half transfer complete callback */
        hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
        
        /* Set the DMA error callback */
        hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;
        
        
        /* Manage ADC and DMA start: ADC overrun interruption, DMA start,     */
        /* ADC start (in case of SW start):                                   */
        
        /* Clear regular group conversion flag and overrun flag               */
        /* (To ensure of no unknown state from potential previous ADC         */
        /* operations)                                                        */
        __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
        
        /* Process unlocked */
        /* Unlock before starting ADC conversions: in case of potential         */
        /* interruption, to let the process to ADC IRQ Handler.                 */
        __HAL_UNLOCK(hadc);
        
        /* With DMA, overrun event is always considered as an error even if 
           hadc->Init.Overrun is set to ADC_OVR_DATA_OVERWRITTEN. Therefore,  
           ADC_IT_OVR is enabled. */
        __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);
        
        /* Enable ADC DMA mode */
        SET_BIT(hadc->Instance->CFGR, ADC_CFGR_DMAEN);
        
        /* Start the DMA channel */
        HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);
        
        /* Enable conversion of regular group.                                  */
        /* If software start has been selected, conversion starts immediately.  */
        /* If external trigger has been selected, conversion will start at next */
        /* trigger event.                                                       */
        /* Start ADC group regular conversion */
        LL_ADC_REG_StartConversion(hadc->Instance);
      }
      else
      {
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
      }
    }
    else
    {
      tmp_hal_status = HAL_ERROR;
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
>>>>>>> Reverted back to single ADC code, testing now
=======
>>>>>>> Got it to compile without having the DMA stuff
  * @brief  Stop ADC conversion of regular group (and injected group in 
  *         case of auto_injection mode), disable ADC DMA transfer, disable 
  *         ADC peripheral.
  * @note:  ADC peripheral disable is forcing stop of potential
  *         conversion on ADC group injected. If ADC group injected is under use, it
  *         should be preliminarily stopped using HAL_ADCEx_InjectedStop function.
  * @note   Case of multimode enabled (when multimode feature is available): 
  *         HAL_ADC_Stop_DMA() function is dedicated to single-ADC mode only. 
  *         For multimode, the dedicated HAL_ADCEx_MultiModeStop_DMA() API must be used.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential ADC group regular conversion on going */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* Disable ADC DMA (ADC DMA configuration of continous requests is kept) */
    CLEAR_BIT(hadc->Instance->CFGR, ADC_CFGR_DMAEN);
    
    /* Disable the DMA channel (in case of DMA in circular mode or stop       */
    /* while DMA transfer is on going)                                        */
    //tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);
    
    /* Check if DMA channel effectively disabled */
    if (tmp_hal_status != HAL_OK)
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
    }
    
    /* Disable ADC overrun interrupt */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_OVR);
    
    /* 2. Disable the ADC peripheral */
    /* Update "tmp_hal_status" only if DMA channel disabling passed, to keep  */
    /* in memory a potential failing status.                                  */
    if (tmp_hal_status == HAL_OK)
    {
      tmp_hal_status = ADC_Disable(hadc);
    }
    else
    {
      ADC_Disable(hadc);
    }

    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
    
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Get ADC regular group conversion result.
  * @note   Reading register DR automatically clears ADC flag EOC
  *         (ADC group regular end of unitary conversion).
  * @note   This function does not clear ADC flag EOS 
  *         (ADC group regular end of sequence conversion).
  *         Occurrence of flag EOS rising:
  *          - If sequencer is composed of 1 rank, flag EOS is equivalent
  *            to flag EOC.
  *          - If sequencer is composed of several ranks, during the scan
  *            sequence flag EOC only is raised, at the end of the scan sequence
  *            both flags EOC and EOS are raised.
  *         To clear this flag, either use function:
  *         in programming model IT: @ref HAL_ADC_IRQHandler(), in programming
  *         model polling: @ref HAL_ADC_PollForConversion()
  *         or @ref __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOS).
  * @param hadc ADC handle
  * @retval ADC group regular conversion data
  */
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Note: EOC flag is not cleared here by software because automatically     */
  /*       cleared by hardware when reading register DR.                      */
  
  /* Return ADC converted value */ 
  return hadc->Instance->DR;
}

/**
  * @brief  Handle ADC interrupt request.
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc)
{
  uint32_t overrun_error = 0; /* flag set if overrun occurrence has to be considered as an error */
  uint32_t tmp_isr = hadc->Instance->ISR;
  uint32_t tmp_ier = hadc->Instance->IER;
  uint32_t tmp_cfgr = 0x0;
  ADC_TypeDef *tmpADC_Master;  
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  
  /* ========== Check End of Sampling flag for ADC group regular ========== */
  if(((tmp_isr & ADC_FLAG_EOSMP) == ADC_FLAG_EOSMP) && ((tmp_ier & ADC_IT_EOSMP) == ADC_IT_EOSMP))
  {
    /* Update state machine on end of sampling status if not in error state */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOSMP);
    }
    
    /* End Of Sampling callback */
      HAL_ADCEx_EndOfSamplingCallback(hadc);
    
    /* Clear regular group conversion flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP );
  }
  
  /* ====== Check ADC group regular end of unitary conversion sequence conversions ===== */
  if((((tmp_isr & ADC_FLAG_EOC) == ADC_FLAG_EOC) && ((tmp_ier & ADC_IT_EOC) == ADC_IT_EOC)) ||
     (((tmp_isr & ADC_FLAG_EOS) == ADC_FLAG_EOS) && ((tmp_ier & ADC_IT_EOS) == ADC_IT_EOS))  )
  {
    /* Update state machine on conversion status if not in error state */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    }
    
    /* Determine whether any further conversion upcoming on group regular     */
    /* by external trigger, continuous mode or scan sequence on going         */
    /* to disable interruption.                                               */
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc))
    {
      /* Get relevant register CFGR in ADC instance of ADC master or slave    */
      /* in function of multimode state (for devices with multimode           */
      /* available).                                                          */
      if (ADC_INDEPENDENT_OR_NONMULTIMODEREGULAR_SLAVE(hadc))
      {
        /* check CONT bit directly in handle ADC CFGR register */
        tmp_cfgr = READ_REG(hadc->Instance->CFGR); 
      }
      else
      {
        /* else need to check Master ADC CONT bit */
        tmpADC_Master = ADC_MASTER_REGISTER(hadc);
        tmp_cfgr = READ_REG(tmpADC_Master->CFGR); 
      }
      
      /* Carry on if continuous mode is disabled */
      if (READ_BIT (tmp_cfgr, ADC_CFGR_CONT) != ADC_CFGR_CONT)
      {
        /* If End of Sequence is reached, disable interrupts */
        if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) )
        {
          /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit         */
          /* ADSTART==0 (no conversion on going)                              */
          if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
          {
            /* Disable ADC end of sequence conversion interrupt */
            /* Note: Overrun interrupt was enabled with EOC interrupt in      */
            /* HAL_Start_IT(), but is not disabled here because can be used   */
            /* by overrun IRQ process below.                                  */
            __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);
            
            /* Set ADC state */
            CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY); 
            
            if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
            { 
              SET_BIT(hadc->State, HAL_ADC_STATE_READY);
            }           
          }
          else
          {
            /* Change ADC state to error state */
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
            
            /* Set ADC error code to ADC IP internal error */
            SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
          }
        }
      }
    }
    
    /* Conversion complete callback */
    /* Note: Into callback function "HAL_ADC_ConvCpltCallback()",             */
    /*       to determine if conversion has been triggered from EOC or EOS,   */
    /*       possibility to use:                                              */
    /*        " if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOS)) "                */
    HAL_ADC_ConvCpltCallback(hadc);
    
    /* Clear regular group conversion flag */
    /* Note: in case of overrun set to ADC_OVR_DATA_PRESERVED, end of         */
    /*       conversion flags clear induces the release of the preserved data.*/
    /*       Therefore, if the preserved data value is needed, it must be     */
    /*       read preliminarily into HAL_ADC_ConvCpltCallback().              */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS) );
  }
  
  /* ====== Check ADC group injected end of unitary conversion sequence conversions ===== */
  if( (((tmp_isr & ADC_FLAG_JEOC) == ADC_FLAG_JEOC) && ((tmp_ier & ADC_IT_JEOC) == ADC_IT_JEOC)) ||
      (((tmp_isr & ADC_FLAG_JEOS) == ADC_FLAG_JEOS) && ((tmp_ier & ADC_IT_JEOS) == ADC_IT_JEOS))  )      
  {
    /* Update state machine on conversion status if not in error state */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_INJ_EOC);
    }
    
    /* Get relevant register CFGR in ADC instance of ADC master or slave  */
    /* in function of multimode state (for devices with multimode         */
    /* available).                                                        */
    if (ADC_INDEPENDENT_OR_NONMULTIMODEINJECTED_SLAVE(hadc))
    {
      tmp_cfgr = READ_REG(hadc->Instance->CFGR); 
    }
    else
    {
      tmpADC_Master = ADC_MASTER_REGISTER(hadc);
      tmp_cfgr = READ_REG(tmpADC_Master->CFGR);
    }
    
    /* Disable interruption if no further conversion upcoming by injected     */
    /* external trigger or by automatic injected conversion with regular      */
    /* group having no further conversion upcoming (same conditions as        */
    /* regular group interruption disabling above),                           */
    /* and if injected scan sequence is completed.                            */
    if(ADC_IS_SOFTWARE_START_INJECTED(hadc)                   ||
       ((READ_BIT (tmp_cfgr, ADC_CFGR_JAUTO) == RESET)    &&
        (ADC_IS_SOFTWARE_START_REGULAR(hadc)          &&
        (READ_BIT (tmp_cfgr, ADC_CFGR_CONT) == RESET)   )   )   )
    {
      /* If End of Sequence is reached, disable interrupts */
      if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS))
      {
        /* Particular case if injected contexts queue is enabled:             */
        /* when the last context has been fully processed, JSQR is reset      */
        /* by the hardware. Even if no injected conversion is planned to come */
        /* (queue empty, triggers are ignored), it can start again            */
        /* immediately after setting a new context (JADSTART is still set).   */
        /* Therefore, state of HAL ADC injected group is kept to busy.        */
        if(READ_BIT(tmp_cfgr, ADC_CFGR_JQM) == RESET)
        {
          /* Allowed to modify bits ADC_IT_JEOC/ADC_IT_JEOS only if bit       */
          /* JADSTART==0 (no conversion on going)                             */
          if (ADC_IS_CONVERSION_ONGOING_INJECTED(hadc) == RESET)
          {
            /* Disable ADC end of sequence conversion interrupt  */
            __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC | ADC_IT_JEOS);
            
            /* Set ADC state */
            CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);

            if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_REG_BUSY))
            { 
              SET_BIT(hadc->State, HAL_ADC_STATE_READY);
            }
          }
          else
          {
            /* Update ADC state machine to error */
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
          
            /* Set ADC error code to ADC IP internal error */
            SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
          }
        }
      }
    }
    
    /* Injected Conversion complete callback */
    /* Note:  HAL_ADCEx_InjectedConvCpltCallback can resort to 
              if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_JEOS)) or
              if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_JEOC)) to determine whether
              interruption has been triggered by end of conversion or end of 
              sequence.    */    
    HAL_ADCEx_InjectedConvCpltCallback(hadc);
    
    /* Clear injected group conversion flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOC | ADC_FLAG_JEOS);
  }
  
  /* ========== Check Analog watchdog 1 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD1) == ADC_FLAG_AWD1) && ((tmp_ier & ADC_IT_AWD1) == ADC_IT_AWD1))      
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    
    /* Level out of window 1 callback */
    HAL_ADC_LevelOutOfWindowCallback(hadc);
    /* Clear ADC analog watchdog flag */ 
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD1);
  }
  
  /* ========== Check analog watchdog 2 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD2) == ADC_FLAG_AWD2) && ((tmp_ier & ADC_IT_AWD2) == ADC_IT_AWD2))      
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD2);
    
    /* Level out of window 2 callback */
    HAL_ADCEx_LevelOutOfWindow2Callback(hadc);
    /* Clear ADC analog watchdog flag */ 
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD2);
  }
  
  /* ========== Check analog watchdog 3 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD3) == ADC_FLAG_AWD3) && ((tmp_ier & ADC_IT_AWD3) == ADC_IT_AWD3))      
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD3);
    
    /* Level out of window 3 callback */
    HAL_ADCEx_LevelOutOfWindow3Callback(hadc);
    /* Clear ADC analog watchdog flag */ 
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD3);
  }
  
  /* ========== Check Overrun flag ========== */
  if (((tmp_isr & ADC_FLAG_OVR) == ADC_FLAG_OVR) && ((tmp_ier & ADC_IT_OVR) == ADC_IT_OVR)) 
  {
    /* If overrun is set to overwrite previous data (default setting),        */
    /* overrun event is not considered as an error.                           */
    /* (cf ref manual "Managing conversions without using the DMA and without */
    /* overrun ")                                                             */
    /* Exception for usage with DMA overrun event always considered as an     */
    /* error.                                                                 */
    
    if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
    {
      overrun_error = 1;
    }
    else
    {
      /* Check DMA configuration */
      if (ADC_IS_DUAL_CONVERSION_ENABLE(hadc) == RESET)
      {
        /* Multimode not set or feature not available or ADC independent */
        if (HAL_IS_BIT_SET(hadc->Instance->CFGR, ADC_CFGR_DMAEN))
        {
          overrun_error = 1;
        }
      }
      else
      {
        /* Multimode (when feature is available) is enabled, 
           Common Control Register MDMA bits must be checked. */
        if (ADC_MULTIMODE_DMA_ENABLED(hadc))
        {
          overrun_error = 1;
        }
      }
    }
        
    if (overrun_error == 1)
    {
      /* Change ADC state to error state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_OVR);
      
      /* Set ADC error code to overrun */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);
      
      /* Error callback */
      /* Note: In case of overrun, ADC conversion data is preserved until     */
      /*       flag OVR is reset.                                             */
      /*       Therefore, old ADC conversion data can be retrieved in         */
      /*       function "HAL_ADC_ErrorCallback()".                            */
      HAL_ADC_ErrorCallback(hadc);
    }
    
    /* Clear ADC overrun flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  }
  
  /* ========== Check Injected context queue overflow flag ========== */
  if (((tmp_isr & ADC_FLAG_JQOVF) == ADC_FLAG_JQOVF) && ((tmp_ier & ADC_IT_JQOVF) == ADC_IT_JQOVF)) 
  {
    /* Change ADC state to overrun state */
    SET_BIT(hadc->State, HAL_ADC_STATE_INJ_JQOVF);
        
    /* Set ADC error code to Injected context queue overflow */
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_JQOVF);
    
    /* Clear the Injected context queue overflow flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JQOVF);
    
    /* Error callback */
    HAL_ADCEx_InjectedQueueOverflowCallback(hadc);
  }
  
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}

/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvHalfCpltCallback must be implemented in the user file.
  */
}

/**
  * @brief  Analog watchdog 1 callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_LevelOutOfWindowCallback must be implemented in the user file.
  */
}

/**
  * @brief  ADC error callback in non-blocking mode
  *         (ADC conversion with interruption or transfer by DMA).
  * @note   In case of error due to overrun when using ADC with DMA transfer 
  *         (HAL ADC handle paramater "ErrorCode" to state "HAL_ADC_ERROR_OVR"):
  *         - Reinitialize the DMA using function "HAL_ADC_Stop_DMA()".
  *         - If needed, restart a new ADC conversion using function
  *           "HAL_ADC_Start_DMA()"
  *           (this function is also clearing overrun flag)
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ErrorCallback must be implemented in the user file.
  */
}

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions 
 *
@verbatim   
 ===============================================================================
             ##### Peripheral Control functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Configure channels on regular group
      (+) Configure the analog watchdog
      
@endverbatim
  * @{
  */

/**
  * @brief  Configure a channel to be assigned to ADC group regular.
  * @note   In case of usage of internal measurement channels:
  *         Vbat/VrefInt/TempSensor.
  *         These internal paths can be disabled using function 
  *         HAL_ADC_DeInit().
  * @note   Possibility to update parameters on the fly:
  *         This function initializes channel into ADC group regular,
  *         following calls to this function can be used to reconfigure
  *         some parameters of structure "ADC_ChannelConfTypeDef" on the fly,
  *         without resetting the ADC.
  *         The setting of these parameters is conditioned to ADC state:
  *         Refer to comments of structure "ADC_ChannelConfTypeDef".
  * @param hadc ADC handle
  * @param sConfig Structure of ADC channel assigned to ADC group regular.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmpOffsetShifted;
  __IO uint32_t wait_loop_index = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_REGULAR_RANK(sConfig->Rank));
  assert_param(IS_ADC_SAMPLE_TIME(sConfig->SamplingTime));
  assert_param(IS_ADC_SINGLE_DIFFERENTIAL(sConfig->SingleDiff));
  assert_param(IS_ADC_OFFSET_NUMBER(sConfig->OffsetNumber));
  assert_param(IS_ADC_RANGE(ADC_GET_RESOLUTION(hadc), sConfig->Offset));
  
  /* if ROVSE is set, the value of the OFFSETy_EN bit in ADCx_OFRy register is  
     ignored (considered as reset) */
  assert_param(!((sConfig->OffsetNumber != ADC_OFFSET_NONE) && (hadc->Init.OversamplingMode == ENABLE)));  
  
  /* Verification of channel number */
  if (sConfig->SingleDiff != ADC_DIFFERENTIAL_ENDED)
  {
     assert_param(IS_ADC_CHANNEL(hadc, sConfig->Channel));
  }
  else
  {
    assert_param(IS_ADC_DIFF_CHANNEL(hadc, sConfig->Channel));
  }
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on regular group:                                    */
  /*  - Channel number                                                        */
  /*  - Channel rank                                                          */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    #if !defined (USE_FULL_ASSERT)
    /* Correspondance for compatibility with legacy definition of             */
    /* sequencer ranks in direct number format. This correspondance can       */
    /* be done only on ranks 1 to 5 due to literal values.                    */
    /* Note: Sequencer ranks in direct number format are no more used         */
    /*       and are detected by activating USE_FULL_ASSERT feature.          */
    if (sConfig->Rank <= 5U)
    {
      switch (sConfig->Rank)
      {
        case 2U: sConfig->Rank = ADC_REGULAR_RANK_2; break;
        case 3U: sConfig->Rank = ADC_REGULAR_RANK_3; break;
        case 4U: sConfig->Rank = ADC_REGULAR_RANK_4; break;
        case 5U: sConfig->Rank = ADC_REGULAR_RANK_5; break;
        /* case 1U */
        default: sConfig->Rank = ADC_REGULAR_RANK_1;
      }
    }
    #endif
    
    /* Set ADC group regular sequence: channel on the selected scan sequence rank */
    LL_ADC_REG_SetSequencerRanks(hadc->Instance, sConfig->Rank, sConfig->Channel);
    
    /* Parameters update conditioned to ADC state:                              */
    /* Parameters that can be updated when ADC is disabled or enabled without   */
    /* conversion on going on regular group:                                    */
    /*  - Channel sampling time                                                 */
    /*  - Channel offset                                                        */
    if (ADC_IS_CONVERSION_ONGOING_REGULAR_INJECTED(hadc) == RESET)
    {
#if defined(ADC_SMPR1_SMPPLUS)
      /* Manage specific case of sampling time 3.5 cycles replacing 2.5 cyles */
      if(sConfig->SamplingTime == ADC_SAMPLETIME_3CYCLES_5)
      {
        /* Set sampling time of the selected ADC channel */
        LL_ADC_SetChannelSamplingTime(hadc->Instance, sConfig->Channel, LL_ADC_SAMPLINGTIME_2CYCLES_5);
        
        /* Set ADC sampling time common configuration */
        LL_ADC_SetSamplingTimeCommonConfig(hadc->Instance, LL_ADC_SAMPLINGTIME_COMMON_3C5_REPL_2C5);
      }
      else
      {
        /* Set sampling time of the selected ADC channel */
        LL_ADC_SetChannelSamplingTime(hadc->Instance, sConfig->Channel, sConfig->SamplingTime);
        
        /* Set ADC sampling time common configuration */
        LL_ADC_SetSamplingTimeCommonConfig(hadc->Instance, LL_ADC_SAMPLINGTIME_COMMON_DEFAULT);
      }
#else
      /* Set sampling time of the selected ADC channel */
      LL_ADC_SetChannelSamplingTime(hadc->Instance, sConfig->Channel, sConfig->SamplingTime);
#endif
      
      /* Configure the offset: offset enable/disable, channel, offset value */

      /* Shift the offset with respect to the selected ADC resolution. */
      /* Offset has to be left-aligned on bit 11, the LSB (right bits) are set to 0 */
      tmpOffsetShifted = ADC_OFFSET_SHIFT_RESOLUTION(hadc, sConfig->Offset);
     
      if(sConfig->OffsetNumber != ADC_OFFSET_NONE)
      {
        /* Set ADC selected offset number */
        LL_ADC_SetOffset(hadc->Instance, sConfig->OffsetNumber, sConfig->Channel, tmpOffsetShifted);
         
      }
      else
      {
        /* Scan each offset register to check if the selected channel is targeted. */
        /* If this is the case, the corresponding offset number is disabled.       */
        if(__LL_ADC_CHANNEL_TO_DECIMAL_NB(LL_ADC_GetOffsetChannel(hadc->Instance, LL_ADC_OFFSET_1)) == __LL_ADC_CHANNEL_TO_DECIMAL_NB(sConfig->Channel))
        {
          LL_ADC_SetOffsetState(hadc->Instance, LL_ADC_OFFSET_1, LL_ADC_OFFSET_DISABLE);
        }
        if(__LL_ADC_CHANNEL_TO_DECIMAL_NB(LL_ADC_GetOffsetChannel(hadc->Instance, LL_ADC_OFFSET_2)) == __LL_ADC_CHANNEL_TO_DECIMAL_NB(sConfig->Channel))
        {
          LL_ADC_SetOffsetState(hadc->Instance, LL_ADC_OFFSET_2, LL_ADC_OFFSET_DISABLE);
        }
        if(__LL_ADC_CHANNEL_TO_DECIMAL_NB(LL_ADC_GetOffsetChannel(hadc->Instance, LL_ADC_OFFSET_3)) == __LL_ADC_CHANNEL_TO_DECIMAL_NB(sConfig->Channel))
        {
          LL_ADC_SetOffsetState(hadc->Instance, LL_ADC_OFFSET_3, LL_ADC_OFFSET_DISABLE);
        }
        if(__LL_ADC_CHANNEL_TO_DECIMAL_NB(LL_ADC_GetOffsetChannel(hadc->Instance, LL_ADC_OFFSET_4)) == __LL_ADC_CHANNEL_TO_DECIMAL_NB(sConfig->Channel))
        {
          LL_ADC_SetOffsetState(hadc->Instance, LL_ADC_OFFSET_4, LL_ADC_OFFSET_DISABLE);
        }
      }
    }
    
    /* Parameters update conditioned to ADC state:                              */
    /* Parameters that can be updated only when ADC is disabled:                */
    /*  - Single or differential mode                                           */
    /*  - Internal measurement channels: Vbat/VrefInt/TempSensor                */
    if (ADC_IS_ENABLE(hadc) == RESET)
    {
      /* Set mode single-ended or differential input of the selected ADC channel */
      LL_ADC_SetChannelSingleDiff(hadc->Instance, sConfig->Channel, sConfig->SingleDiff);
      
      /* Configuration of differential mode */
      if (sConfig->SingleDiff == ADC_DIFFERENTIAL_ENDED)
      {
        /* Set sampling time of the selected ADC channel */
        LL_ADC_SetChannelSamplingTime(hadc->Instance, __LL_ADC_DECIMAL_NB_TO_CHANNEL(__LL_ADC_CHANNEL_TO_DECIMAL_NB(sConfig->Channel) + 1), sConfig->SamplingTime);
      }
      
      /* Management of internal measurement channels: Vbat/VrefInt/TempSensor.  */
      /* If internal channel selected, enable dedicated internal buffers and    */
      /* paths.                                                                 */
      /* Note: these internal measurement paths can be disabled using           */
      /* HAL_ADC_DeInit().                                                      */
         
      /* Configuration of common ADC parameters                                 */
      /* If the requested internal measurement path has already been enabled,   */
      /* bypass the configuration processing.                                   */
      if (( (sConfig->Channel == ADC_CHANNEL_TEMPSENSOR) &&
            ((LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) & LL_ADC_PATH_INTERNAL_TEMPSENSOR) == 0U)) ||
          ( (sConfig->Channel == ADC_CHANNEL_VBAT)       &&
            ((LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) & LL_ADC_PATH_INTERNAL_VBAT) == 0U))       ||
          ( (sConfig->Channel == ADC_CHANNEL_VREFINT)    &&
            ((LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) & LL_ADC_PATH_INTERNAL_VREFINT) == 0U))
         )
      {
        /* Configuration of common ADC parameters (continuation)                */
  
        /* Software is allowed to change common parameters only when all ADCs   */
        /* of the common group are disabled.                                    */
        if ((ADC_IS_ENABLE(hadc) == RESET)   &&
           (ADC_ANY_OTHER_ENABLED(hadc) == RESET) )
        {
          if (sConfig->Channel == ADC_CHANNEL_TEMPSENSOR)
          {
            if (ADC_TEMPERATURE_SENSOR_INSTANCE(hadc)) 
            {
              LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance), LL_ADC_PATH_INTERNAL_TEMPSENSOR | LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)));
              
              /* Delay for temperature sensor stabilization time */
              /* Wait loop initialization and execution */
              /* Note: Variable divided by 2 to compensate partially          */
              /*       CPU processing cycles.                                 */
              wait_loop_index = (LL_ADC_DELAY_TEMPSENSOR_STAB_US * (SystemCoreClock / (1000000 * 2)));
              while(wait_loop_index != 0)
              {
                wait_loop_index--;
              }
            }
          }
          else if (sConfig->Channel == ADC_CHANNEL_VBAT)
          {
            if (ADC_BATTERY_VOLTAGE_INSTANCE(hadc))
            {
              LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance), LL_ADC_PATH_INTERNAL_VBAT | LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)));
            }
          }
          else if (sConfig->Channel == ADC_CHANNEL_VREFINT)
          {
            if (ADC_VREFINT_INSTANCE(hadc))
            {
              LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance), LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance)));
            }
          }
        }
        /* If the requested internal measurement path has already been          */
        /* enabled and other ADC of the common group are enabled, internal      */
        /* measurement paths cannot be enabled.                                 */
        else  
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
          
          tmp_hal_status = HAL_ERROR;
        }
      }
    }
  }
  
  /* If a conversion is on going on regular group, no update on regular       */
  /* channel could be done on neither of the channel configuration structure  */
  /* parameters.                                                              */
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    tmp_hal_status = HAL_ERROR;
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Configure the analog watchdog.
  * @note   Possibility to update parameters on the fly:
  *         This function initializes the selected analog watchdog, successive  
  *         calls to this function can be used to reconfigure some parameters 
  *         of structure "ADC_AnalogWDGConfTypeDef" on the fly, without resetting 
  *         the ADC.
  *         The setting of these parameters is conditioned to ADC state.
  *         For parameters constraints, see comments of structure 
  *         "ADC_AnalogWDGConfTypeDef".
  * @note   On this STM32 serie, analog watchdog thresholds cannot be modified
  *         while ADC conversion is on going.
  * @param hadc ADC handle
  * @param AnalogWDGConfig Structure of ADC analog watchdog configuration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmpAWDHighThresholdShifted = 0U;
  uint32_t tmpAWDLowThresholdShifted = 0U;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_ANALOG_WATCHDOG_NUMBER(AnalogWDGConfig->WatchdogNumber));
  assert_param(IS_ADC_ANALOG_WATCHDOG_MODE(AnalogWDGConfig->WatchdogMode));
  assert_param(IS_FUNCTIONAL_STATE(AnalogWDGConfig->ITMode));
  
  if((AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REG)     ||
     (AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_INJEC)   ||
     (AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REGINJEC)  )
  {
    assert_param(IS_ADC_CHANNEL(hadc, AnalogWDGConfig->Channel));
  }

  /* Verify if threshold is within the selected ADC resolution */
  assert_param(IS_ADC_RANGE(ADC_GET_RESOLUTION(hadc), AnalogWDGConfig->HighThreshold));
  assert_param(IS_ADC_RANGE(ADC_GET_RESOLUTION(hadc), AnalogWDGConfig->LowThreshold));

  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on ADC groups regular and injected:                  */
  /*  - Analog watchdog channels                                              */
  /*  - Analog watchdog thresholds                                            */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR_INJECTED(hadc) == RESET)
  {
    /* Analog watchdog configuration */
    if(AnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_1)
    {
      /* Configuration of analog watchdog:                                    */
      /*  - Set the analog watchdog enable mode: one or overall group of      */
      /*    channels, on groups regular and-or injected.                      */
      switch(AnalogWDGConfig->WatchdogMode)
      {
        case ADC_ANALOGWATCHDOG_SINGLE_REG:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, __LL_ADC_ANALOGWD_CHANNEL_GROUP(AnalogWDGConfig->Channel, LL_ADC_GROUP_REGULAR));
          break;
        
        case ADC_ANALOGWATCHDOG_SINGLE_INJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, __LL_ADC_ANALOGWD_CHANNEL_GROUP(AnalogWDGConfig->Channel, LL_ADC_GROUP_INJECTED));
          break;
        
        case ADC_ANALOGWATCHDOG_SINGLE_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, __LL_ADC_ANALOGWD_CHANNEL_GROUP(AnalogWDGConfig->Channel, LL_ADC_GROUP_REGULAR_INJECTED));
          break;
        
        case ADC_ANALOGWATCHDOG_ALL_REG:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG);
          break;
        
        case ADC_ANALOGWATCHDOG_ALL_INJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_INJ);
          break;
        
        case ADC_ANALOGWATCHDOG_ALL_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG_INJ);
          break;
        
        default: /* ADC_ANALOGWATCHDOG_NONE */
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_DISABLE);
          break;
      }
      
      /* Shift the offset in function of the selected ADC resolution:         */
      /* Thresholds have to be left-aligned on bit 11, the LSB (right bits)   */
      /* are set to 0                                                         */ 
      tmpAWDHighThresholdShifted = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->HighThreshold);
      tmpAWDLowThresholdShifted  = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->LowThreshold);
      
      /* Set ADC analog watchdog thresholds value of both thresholds high and low */
      LL_ADC_ConfigAnalogWDThresholds(hadc->Instance, AnalogWDGConfig->WatchdogNumber, tmpAWDHighThresholdShifted, tmpAWDLowThresholdShifted);
      
      /* Update state, clear previous result related to AWD1 */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD1);
      
      /* Clear flag ADC analog watchdog */
      /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
      /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
      /* (in case left enabled by previous ADC operations).                 */
      LL_ADC_ClearFlag_AWD1(hadc->Instance);
      
      /* Configure ADC analog watchdog interrupt */
      if(AnalogWDGConfig->ITMode == ENABLE)
      {
        LL_ADC_EnableIT_AWD1(hadc->Instance);
      }
      else
      {
        LL_ADC_DisableIT_AWD1(hadc->Instance);
      }
    }
    /* Case of ADC_ANALOGWATCHDOG_2 or ADC_ANALOGWATCHDOG_3 */
    else
    {
      switch(AnalogWDGConfig->WatchdogMode)
      {
        case ADC_ANALOGWATCHDOG_SINGLE_REG:
        case ADC_ANALOGWATCHDOG_SINGLE_INJEC:
        case ADC_ANALOGWATCHDOG_SINGLE_REGINJEC:
          /* Update AWD by bitfield to keep the possibility to monitor        */
          /* several channels by successive calls of this function.           */
          if (AnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_2)
          {
            SET_BIT(hadc->Instance->AWD2CR, (1U << __LL_ADC_CHANNEL_TO_DECIMAL_NB(AnalogWDGConfig->Channel)));
          }
          else
          {
            SET_BIT(hadc->Instance->AWD3CR, (1U << __LL_ADC_CHANNEL_TO_DECIMAL_NB(AnalogWDGConfig->Channel)));
          }
          break;
          
        case ADC_ANALOGWATCHDOG_ALL_REG:
        case ADC_ANALOGWATCHDOG_ALL_INJEC:
        case ADC_ANALOGWATCHDOG_ALL_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, AnalogWDGConfig->WatchdogNumber, LL_ADC_AWD_ALL_CHANNELS_REG_INJ);
          break;
          
        default: /* ADC_ANALOGWATCHDOG_NONE */
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, AnalogWDGConfig->WatchdogNumber, LL_ADC_AWD_DISABLE);
          break;
      }
      
      /* Shift the thresholds in function of the selected ADC resolution      */
      /* have to be left-aligned on bit 7, the LSB (right bits) are set to 0  */
      tmpAWDHighThresholdShifted = ADC_AWD23THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->HighThreshold);
      tmpAWDLowThresholdShifted  = ADC_AWD23THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->LowThreshold);
      
      /* Set ADC analog watchdog thresholds value of both thresholds high and low */
      LL_ADC_ConfigAnalogWDThresholds(hadc->Instance, AnalogWDGConfig->WatchdogNumber, tmpAWDHighThresholdShifted, tmpAWDLowThresholdShifted);
      
      if (AnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_2)
      {
        /* Update state, clear previous result related to AWD2 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD2);
        
        /* Clear flag ADC analog watchdog */
        /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
        /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
        /* (in case left enabled by previous ADC operations).                 */
        LL_ADC_ClearFlag_AWD2(hadc->Instance);
        
        /* Configure ADC analog watchdog interrupt */
        if(AnalogWDGConfig->ITMode == ENABLE)
        {
          LL_ADC_EnableIT_AWD2(hadc->Instance);
        }
        else
        {
          LL_ADC_DisableIT_AWD2(hadc->Instance);
        }
      }
      /* (AnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_3) */
      else
      {
        /* Update state, clear previous result related to AWD3 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD3);
        
        /* Clear flag ADC analog watchdog */
        /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
        /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
        /* (in case left enabled by previous ADC operations).                 */
        LL_ADC_ClearFlag_AWD3(hadc->Instance);
        
        /* Configure ADC analog watchdog interrupt */
        if(AnalogWDGConfig->ITMode == ENABLE)
        {
          LL_ADC_EnableIT_AWD3(hadc->Instance);
        }
        else
        {
          LL_ADC_DisableIT_AWD3(hadc->Instance);
        }
      }
    }
    
  }
  /* If a conversion is on going on ADC group regular or injected, no update  */
  /* could be done on neither of the AWD configuration structure parameters.  */
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    tmp_hal_status = HAL_ERROR;
  }
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}


/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group4 Peripheral State functions
 *  @brief    ADC Peripheral State functions
 *
@verbatim
 ===============================================================================
            ##### Peripheral state and errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions to get in run-time the status of the  
    peripheral.
      (+) Check the ADC state
      (+) Check the ADC error code

@endverbatim
  * @{
  */

/**
  * @brief  Return the ADC handle state.
  * @note   ADC state machine is managed by bitfields, ADC status must be 
  *         compared with states bits.
  *         For example:                                                         
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_REG_BUSY)) "
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD1)    ) "
  * @param hadc ADC handle
  * @retval ADC handle state (bitfield on 32 bits)
  */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Return ADC handle state */
  return hadc->State;
}

/**
  * @brief  Return the ADC error code.
  * @param hadc ADC handle
  * @retval ADC error code (bitfield on 32 bits)
  */
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  return hadc->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Functions ADC Private Functions
  * @{
  */

/**
  * @brief  Stop ADC conversion.
  * @param hadc ADC handle
  * @param ConversionGroup ADC group regular and/or injected.
  *          This parameter can be one of the following values:
  *            @arg @ref ADC_REGULAR_GROUP           ADC regular conversion type.
  *            @arg @ref ADC_INJECTED_GROUP          ADC injected conversion type.
  *            @arg @ref ADC_REGULAR_INJECTED_GROUP  ADC regular and injected conversion type.
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef* hadc, uint32_t ConversionGroup)
{
  uint32_t tmp_ADC_CR_ADSTART_JADSTART = 0;
  uint32_t tickstart = 0;
  uint32_t Conversion_Timeout_CPU_cycles = 0;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CONVERSION_GROUP(ConversionGroup));
    
  /* Verification if ADC is not already stopped (on regular and injected      */
  /* groups) to bypass this function if not needed.                           */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR_INJECTED(hadc))
  {
    /* Particular case of continuous auto-injection mode combined with        */
    /* auto-delay mode.                                                       */
    /* In auto-injection mode, regular group stop ADC_CR_ADSTP is used (not   */
    /* injected group stop ADC_CR_JADSTP).                                    */
    /* Procedure to be followed: Wait until JEOS=1, clear JEOS, set ADSTP=1   */
    /* (see reference manual).                                                */
    if ((HAL_IS_BIT_SET(hadc->Instance->CFGR, ADC_CFGR_JAUTO)) 
         && (hadc->Init.ContinuousConvMode==ENABLE) 
         && (hadc->Init.LowPowerAutoWait==ENABLE))
    {
      /* Use stop of regular group */
      ConversionGroup = ADC_REGULAR_GROUP;
      
      /* Wait until JEOS=1 (maximum Timeout: 4 injected conversions) */
      while(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS) == RESET)
      {
        if (Conversion_Timeout_CPU_cycles >= (ADC_CONVERSION_TIME_MAX_CPU_CYCLES *4))
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
          
          /* Set ADC error code to ADC IP internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
          
          return HAL_ERROR;
        }
        Conversion_Timeout_CPU_cycles ++;
      }
      
      /* Clear JEOS */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOS);
    }
    
    /* Stop potential conversion on going on regular group */
    if (ConversionGroup != ADC_INJECTED_GROUP)
    {
      /* Software is allowed to set ADSTP only when ADSTART=1 and ADDIS=0 */
      if (HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_ADSTART) && 
          HAL_IS_BIT_CLR(hadc->Instance->CR, ADC_CR_ADDIS)     )
      {
        /* Stop conversions on regular group */
        LL_ADC_REG_StopConversion(hadc->Instance);
      }
    }
    
    /* Stop potential conversion on going on injected group */
    if (ConversionGroup != ADC_REGULAR_GROUP)
    {
      /* Software is allowed to set JADSTP only when JADSTART=1 and ADDIS=0 */
      if (HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_JADSTART) && 
          HAL_IS_BIT_CLR(hadc->Instance->CR, ADC_CR_ADDIS)      )
      {
        /* Stop conversions on injected group */
        SET_BIT(hadc->Instance->CR, ADC_CR_JADSTP);
      }   
    }
    
    /* Selection of start and stop bits with respect to the regular or injected group */
    switch(ConversionGroup)
    {
    case ADC_REGULAR_INJECTED_GROUP:
        tmp_ADC_CR_ADSTART_JADSTART = (ADC_CR_ADSTART | ADC_CR_JADSTART);
        break;
    case ADC_INJECTED_GROUP:
        tmp_ADC_CR_ADSTART_JADSTART = ADC_CR_JADSTART;
        break;
    /* Case ADC_REGULAR_GROUP only*/
    default:
        tmp_ADC_CR_ADSTART_JADSTART = ADC_CR_ADSTART;
        break;
    }
    
    /* Wait for conversion effectively stopped */
    
    
    tickstart = HAL_GetTick();
      
    while((hadc->Instance->CR & tmp_ADC_CR_ADSTART_JADSTART) != RESET)
    {
      if((HAL_GetTick()-tickstart) > ADC_STOP_CONVERSION_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Set ADC error code to ADC IP internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
    
  }
  
  /* Return HAL status */
  return HAL_OK;
}



/**
  * @brief  Enable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC must be disabled
  *         and voltage regulator must be enabled (done into HAL_ADC_Init()).
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0;
  __IO uint32_t wait_loop_index = 0;
  
  /* ADC enable and wait for ADC ready (in case of ADC is disabled or         */
  /* enabling phase not yet completed: flag ADC ready not yet set).           */
  /* Timeout implemented to not be stuck if ADC cannot be enabled (possible   */
  /* causes: ADC clock not running, ...).                                     */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {
    /* Check if conditions to enable the ADC are fulfilled */
    if (ADC_ENABLING_CONDITIONS(hadc) == RESET)
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
      
      /* Set ADC error code to ADC IP internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      
      return HAL_ERROR;
    }
    
    /* Enable the ADC peripheral */
    LL_ADC_Enable(hadc->Instance);
    
    /* Delay for ADC stabilization time */
    /* Wait loop initialization and execution */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles.                                           */
    wait_loop_index = (LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (1000000 * 2)));
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }

    /* Wait for ADC effectively enabled */
    tickstart = HAL_GetTick();
    
    while(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_RDY) == RESET)
    {
      /*  If ADEN bit is set less than 4 ADC clock cycles after the ADCAL bit 
          has been cleared (after a calibration), ADEN bit is reset by the 
          calibration logic.
          The workaround is to continue setting ADEN until ADRDY is becomes 1.
          Additionally, ADC_ENABLE_TIMEOUT is defined to encompass this
          4 ADC clock cycle duration */
      /* Note: Test of ADC enabled required due to hardware constraint to     */
      /*       not enable ADC if already enabled.                             */
      if(LL_ADC_IsEnabled(hadc->Instance) == 0)
      {
        LL_ADC_Enable(hadc->Instance);
      }
      
      if((HAL_GetTick() - tickstart) > ADC_ENABLE_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Set ADC error code to ADC IP internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
  }
   
  /* Return HAL status */
  return HAL_OK;
}

/**
  * @brief  Disable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC conversions must be
  *         stopped.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0;
  
  /* Verification if ADC is not already disabled:                             */
  /* Note: forbidden to disable ADC (set bit ADC_CR_ADDIS) if ADC is already  */
  /*       disabled.                                                          */
  if (ADC_IS_ENABLE(hadc) != RESET)
  {
    /* Check if conditions to disable the ADC are fulfilled */
    if (ADC_DISABLING_CONDITIONS(hadc) != RESET)
    {
      /* Disable the ADC peripheral */
      LL_ADC_Disable(hadc->Instance);
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOSMP | ADC_FLAG_RDY));
    }
    else
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
      
      /* Set ADC error code to ADC IP internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      
      return HAL_ERROR;
    }
     
    /* Wait for ADC effectively disabled */
    /* Get tick count */
    tickstart = HAL_GetTick();
    
    while(HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_ADEN))
    {
      if((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Set ADC error code to ADC IP internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
  }
  
  /* Return HAL status */
  return HAL_OK;
}


/**
  * @}
  */

#endif /* HAL_ADC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
