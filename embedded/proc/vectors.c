typedef void(*f_ptr)(void);

/* GCC won't let me reference Default_Handler . . . */
void _Default_Handler(void) {
	/* TODO */
	while(1);
}

void WWDG_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void PVD_PVM_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TAMP_STAMP_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void RTC_WKUP_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void ADC1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void CAN1_TX_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM1_BRK_TIM15_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM1_UP_TIM16_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM6_DAC_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel3_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel4_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel5_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void COMP_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void LPTIM1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void LPTIM2_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void USB_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel6_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void DMA2_Channel7_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void LPUART1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void QUADSPI_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void I2C3_EV_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void I2C3_ER_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void SAI1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void SWPMI1_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void TSC_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void RNG_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void FPU_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));
void CRS_IRQHandler(void) __attribute__((weak, alias("_Default_Handler")));

const f_ptr __dev_vectors[] __attribute__((section(".device_vectors"))) = {
	WWDG_IRQHandler,
	PVD_PVM_IRQHandler,
	TAMP_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_IRQHandler,
	CAN1_TX_IRQHandler,
	CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM15_IRQHandler,
	TIM1_UP_TIM16_IRQHandler,
	TIM1_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	0, 0,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	0, 0,
	SPI1_IRQHandler,
	0,
	USART1_IRQHandler,
	USART2_IRQHandler,
	0,
	EXTI15_10_IRQHandler,
	RTC_Alarm_IRQHandler,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	SPI3_IRQHandler,
	0, 0,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_IRQHandler,
	DMA2_Channel5_IRQHandler,
	0, 0, 0,
	COMP_IRQHandler,
	LPTIM1_IRQHandler,
	LPTIM2_IRQHandler,
	USB_IRQHandler,
	DMA2_Channel6_IRQHandler,
	DMA2_Channel7_IRQHandler,
	LPUART1_IRQHandler,
	QUADSPI_IRQHandler,
	I2C3_EV_IRQHandler,
	I2C3_ER_IRQHandler,
	SAI1_IRQHandler,
	0,
	SWPMI1_IRQHandler,
	TSC_IRQHandler,
	0, 0,
	RNG_IRQHandler,
	FPU_IRQHandler,
	CRS_IRQHandler
};
