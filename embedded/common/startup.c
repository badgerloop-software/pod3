#include <stdint.h>

extern uint32_t __etext;
extern uint32_t __data_start__, __data_end__;
extern uint32_t __copy_table_start__, __copy_table_end__;
extern uint32_t __zero_table_start__, __zero_table_end__;
extern uint32_t __bss_start__, __bss_end__;
extern uint32_t __StackTop;

#ifndef __STACK_SIZE
#define	__STACK_SIZE	0x00000400
#endif
static uint8_t stack[__STACK_SIZE] __attribute__((
	aligned(8), used, section(".stack")
));

#ifndef __HEAP_SIZE
#define	__HEAP_SIZE		0x00000C00
#endif
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE] __attribute__((
	aligned(8), used, section(".heap")
));
#endif

extern void SystemInit(void);

#ifndef __START
/* PreeMain (C library entry point) */
extern void  _start(void) __attribute__((noreturn));
#else
/* main entry point */
extern int  __START(void) __attribute__((noreturn));
#endif

void Reset_Handler(void) {

	uint32_t *pSrc, *pDest;
	uint32_t *pTable __attribute__((unused));

	pSrc  = &__etext;
	pDest = &__data_start__;

	/* copy data section from ROM image to RAM */
	for ( ; pDest < &__data_end__ ; )
		*pDest++ = *pSrc++;

	pDest = &__bss_start__;

	/* zero BSS */
	for ( ; pDest < &__bss_end__ ; )
		*pDest++ = 0UL;

	SystemInit();

#ifndef __START
#define __START _start
#endif
	__START();
}

void Default_Handler(void) {
	/* TODO */
	while(1);
}

void NMI_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler  (void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler (void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler        (void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler   (void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler     (void) __attribute__((weak, alias("Default_Handler")));

volatile unsigned int ticks = 0;
void SysTick_Handler(void) { ticks++; }

typedef void(*f_ptr)(void);
const f_ptr __core_vectors[] __attribute__((section(".vectors"))) = {
	(f_ptr)((uint32_t)&__StackTop),	/*	Initial Stack Pointer	*/
	Reset_Handler,					/*	Reset Handler			*/
	NMI_Handler,					/*	NMI Handler				*/
	HardFault_Handler,				/*	Hard Fault Handler		*/
	MemManage_Handler,				/*	MPU Fault Handler		*/
	BusFault_Handler,				/*	Bus Fault Handler		*/
	UsageFault_Handler,				/*	Usage Fault Handler		*/
	0,								/*	Reserved				*/
	0,								/*	Reserved				*/
	0,								/*	Reserved				*/
	0,								/*	Reserved				*/
	SVC_Handler,					/*	SVCall Handler			*/
	DebugMon_Handler,				/*	Debug Monitor Handler	*/
	0,								/*	Reserved				*/
	PendSV_Handler,					/*	PendSV Handler			*/
	SysTick_Handler,				/*	SysTick Handler			*/
};
