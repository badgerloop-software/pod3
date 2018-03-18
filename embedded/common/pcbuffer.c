#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "pcbuffer.h"

bool pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size) {

	if (!buffer) return false;

	void *temp = malloc(sizeof(char)*buffer_size);
	if (!temp) return false;
	buffer->array = temp;
	buffer->buffer_SIZE = buffer_size;
	buffer->consume_count = 0;
	buffer->produce_count = 0;
	buffer->message_available = 0;
	return true;
}

void pc_buffer_add(PC_Buffer *buf, char data) {
	buf->array[buf->produce_count++ % buf->buffer_SIZE] = data;
}

void pc_buffer_remove(PC_Buffer *buf, char *data) {
	*data = buf->array[buf->consume_count++ % buf->buffer_SIZE];
}

bool pc_buffer_empty(PC_Buffer *buf) {
	return buf->produce_count == buf->consume_count;
}

bool pc_buffer_full(PC_Buffer *buf) {
	return buf->produce_count - buf->consume_count == buf->buffer_SIZE;
}

bool pc_buffer_messageAvailable(PC_Buffer *buf) {
	return buf->message_available > 0;
}

bool pc_buffer_getMessage(PC_Buffer *buffer, char *message, int maxLength) {

	char curr = '\0', prev = '\0';
	int index = 0;
	
	if (!buffer->message_available) return false;
	
	while (index < maxLength && !pc_buffer_empty(buffer)) {
		prev = curr;
		pc_buffer_remove(buffer, &curr);
		message[index++] = NEWLINE_GUARD(curr, prev) ? '\0' : curr;
		if (NEWLINE_GUARD(curr, prev)) {
			buffer->message_available--;
			return true;
		}
	}

	return false;
}
