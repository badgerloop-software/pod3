#ifndef _STATE_MACHINE__H__
#define _STATE_MACHINE__H__


#include <stdint.h>
#include <stdbool.h>

#define NUM_STATES 13

extern volatile unsigned int ticks;

typedef enum state_name {
	PRE_RUN_FAULT = 0,
	RUN_FAULT = 1,
	POST_RUN_FAULT = 2,
	IDLE = 3,
	READY_FOR_PUMPDOWN = 4,
	PUMPDOWN = 5,
	READY = 6,
	PROPULSION_START = 7,
	PROPULSION_DISTANCE = 8,
	BRAKING = 9,
	POST_RUN = 10,
	SAFE_TO_APPROACH = 11,
	SERVICE_LOW_SPEED_PROPULSION = 12
} STATE_NAME;

typedef void state_transition_t(STATE_NAME to_or_from, uint32_t flags);
typedef void state_handler_t(uint32_t flags);

typedef struct state{
	STATE_NAME curr_state;
	STATE_NAME prev_state;
	STATE_NAME next_state;
	volatile bool change_state;
	volatile uint32_t flags;

#define POWER_ON 0x00000001
#define RUN_OVER 0x80000000
#define RETRY_INIT 0x00000002

	state_transition_t **to_state_table;
	state_handler_t **in_state_table;
	state_transition_t **from_state_table;
	unsigned int *state_timestamp_table;
	const unsigned int *event_interval_table;



}state_t;



extern const char *state_strings[];
extern const char *fault_message;
extern state_t state_handle;
// Start of each state
extern unsigned int state_event_timestamps[NUM_STATES];
extern const unsigned int state_intervals[];
#define GET_TIMESTAMP(state)	\
	(state_handle.state_timestamp_table[state])
#define SET_TIMESTAMP(state)	\
	state_handle.state_timestamp_table[state] = ticks
#define GET_INTERVAL(state)	\
	(state_handle.event_interval_table[state])


extern state_transition_t * to_handlers[];
extern state_handler_t * in_handlers[];
extern state_transition_t * from_handlers[];

void initialize_state_machine(STATE_NAME initial_state);
void state_machine_logic();
void state_machine_handler();
int check_interval(STATE_NAME state);

#endif
