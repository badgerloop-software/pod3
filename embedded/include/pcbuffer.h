#ifndef __PC_BUFFER_H__
#define __PC_BUFFER_H__

#define NEWLINE_GUARD   (curr == '\n' && prev != '\r') || \
						(curr == '\r' && prev != '\n')

typedef struct {
	uint32_t produce_count, consume_count;
	uint16_t buffer_SIZE, message_available;
	char *array;
} PC_Buffer;

bool pc_buffer_init(PC_Buffer *buffer, uint16_t buffer_size);
void pc_buffer_add(PC_Buffer *buffer, char data);
void pc_buffer_remove(PC_Buffer *buffer, char *data);
bool pc_buffer_empty(PC_Buffer *buffer);
bool pc_buffer_full(PC_Buffer *buffer);
bool pc_buffer_getMessage(PC_Buffer *buffer, char *message, int maxLength);
bool pc_buffer_messageAvailable(PC_Buffer *buffer);

#endif
