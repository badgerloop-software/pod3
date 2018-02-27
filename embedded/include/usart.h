#ifndef _UART__H__
#define _UART__H__

#include <stdio.h>
#include <stdbool.h>
#include "system.h"
#include "pcbuffer.h"

#define USART_BUF		128
#define USART_INT_PRIO	4

typedef enum {
	APBX =		0,
	SYSCLK =	1,
	HSI_SRC =	2,
	LSE_SRC =	3
} usart_clk_src_t;
			
int usart_config(USART_TypeDef* usart, usart_clk_src_t src, uint32_t control[3], uint32_t baud, bool ie);

extern PC_Buffer *tx_buf[3], *rx_buf[3];
#define USB_BUF_IND	1
#define USB_TX		tx_buf[USB_BUF_IND]
#define USB_RX		tx_buf[USB_BUF_IND]

#endif
