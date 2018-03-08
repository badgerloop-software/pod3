/*
 * stdio redirection
 */

#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"

#define BLOCK	true

int _write(int fd, const void *buf, size_t count) {
	for (fd = 0; (size_t) fd < count; fd++) {
		if (_putc(USB_UART, BLOCK, *((char *) buf++)))
			return fd;
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
