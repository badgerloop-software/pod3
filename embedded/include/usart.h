#ifndef _UART__H__
#define _UART__H__

#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "pcbuffer.h"

#define USART_BUF		128
#define USART_INT_PRIO	4

#define USB_UART	USART2

typedef enum {
	APBX =		0,
	SYSCLK =	1,
	HSI_SRC =	2,
	LSE_SRC =	3
} usart_clk_src_t;
			
int usart_config(
	USART_TypeDef* usart, usart_clk_src_t src, uint32_t control[3],
	uint32_t baud, bool ie
);

int _getc(USART_TypeDef* usart, bool block, char *c);
int _putc(USART_TypeDef* usart, bool block, char data);
PC_Buffer *get_tx(USART_TypeDef* usart);
PC_Buffer *get_rx(USART_TypeDef* usart);

extern PC_Buffer *tx_buf[3], *rx_buf[3];

#endif
