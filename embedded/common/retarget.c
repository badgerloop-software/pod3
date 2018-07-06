/*
 * stdio redirection
 */

#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"

#define BLOCK	true

int _write(int fd, const void *buf, size_t count) {
	int i;
	if (fd == 2) {
		for (i = 0; (size_t) i < count; i++) {
			if(_putc(USART1, BLOCK, *((char *) buf++)))
				return i;
		}
	} else if (fd == 1) {
		for (fd = 0; (size_t) fd < count; fd++) {
			if (_putc(USB_UART, BLOCK, *((char *) buf++)))
				return fd;
		}
	} else {
		return count;
	}
	return count;
}

int _read(int fd, const void *buf, size_t count) {
	for (fd = 0; (size_t) fd < count; fd++) {
		if (_getc(USB_UART, BLOCK, (char *) buf++))
			return fd;
	}
	return count;
}
