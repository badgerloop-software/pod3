#ifndef _DASHBOARD_CONTROL__H__
#define _DASHBOARD_CONTROL__H__

#include "pcbuffer.h"

void check_incoming_controls(PC_Buffer *rx);
void process_control(char *buf);

#endif
