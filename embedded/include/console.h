#ifndef _CONSOLE__H__
#define _CONSOLE__H__

#include "pcbuffer.h"

#define MAX_TOKENS	16

void check_input(PC_Buffer *rx);
void process_input(char *buf);
void printPrompt(void);

#endif
