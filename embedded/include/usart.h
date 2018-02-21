#ifndef _UART__H__
#define _UART__H__

#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "pcbuffer.h"

#define USART_BUF	128

extern volatile bool upArrowFlag, downArrowFlag;

typedef enum {
	APB1 =		0,
	SYSCLK =	1,
	HSI_SRC =	2,
	LSE_SRC =	3
} USART_CLK_SRC;
			
int usart_config(USART_TypeDef* usart, USART_CLK_SRC src, uint32_t control[3], uint32_t baud, bool ie);

#endif
