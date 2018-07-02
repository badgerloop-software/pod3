#ifndef _STATE_HANDLERS_H__
#define _STATE_HANDLERS_H__

#include "state_machine.h"

uint32_t propulsion_timeout_ms;

void change_state(STATE_NAME state);
void set_propulsion_timeout(uint32_t timeout_ms);

#endif
