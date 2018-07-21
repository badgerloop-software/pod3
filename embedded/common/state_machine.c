#include <stdio.h>
#include "state_machine.h"
#include "state_handlers.h"
#include "nav_data.h"
#include "pv_data.h"

/* Globals */
state_t state_handle;
const char *fault_message = "INITIAL_VALUE";
state_box stateVal;
state_box pv_stateVal;
/**
  * @p STATE_NAME state, checking time in current state
  * Basic functionality of states, call check_interval for determination if we
	* should change states. For example, PROPULSION_START cannot last longer than
	* 5 seconds, if it has been 5 seconds, move to the next state
	* ONLY used by main state machine board
  */
//int check_interval(STATE_NAME state){
//	// State cannot be switched out of by timer
//	if(GET_INTERVAL(state) == 9999) return 0;
//	unsigned int state_start = GET_TIMESTAMP(state);
//
//	// 1 tick = 1 ms
//	if(ticks - state_start >= GET_INTERVAL(state)){
//		// The time limit for a state has been reached
//		return 1;
//	}
//	return 0;
//}
//*/

void initialize_state_machine(STATE_NAME initial_state) {

	state_t *handle = &state_handle;

	/* Set initial state */
	handle->prev_state = initial_state; 
	handle->curr_state = initial_state;
	handle->next_state = initial_state;

	/* Set up table pointers */
	handle->to_state_table = to_handlers;
	handle->in_state_table = in_handlers;
	handle->from_state_table = from_handlers;

	/* Timestamp table for control over event frequency */
	handle->state_timestamp_table = state_event_timestamps;
//	handle->event_interval_table = state_intervals;

	/* No flags, no state change assertion */
	handle->change_state = false;
	handle->flags = POWER_ON;

}

/** Performs the logic on when to be in what state */
void state_machine_logic() {
	STATE_NAME cState = state_handle.curr_state;

	// Check if timer expired for state
	if(check_interval(cState)) {
		change_state(++cState);
		return;
	}
	
}

/**
  * "Dumb" handler of states. This function is called by all boards to update
	* states. The board in charge of  logic calls a seperate function first then
	* this one.
  */
void state_machine_handler(){
	state_t *handle = &state_handle;
	
	if (stateVal.change_state) {
		handle->next_state = stateVal.stateName;
		handle->change_state = true;
		stateVal.change_state = 0;
	}

	if (pv_stateVal.change_state) {
		handle->next_state = pv_stateVal.stateName;
		handle->change_state = true;
		pv_stateVal.change_state = 0;
	}
	/* Enter state handler */
	handle->in_state_table[handle->curr_state](handle->flags);

	/* Check if we are transitioning */
	if (handle->change_state) {
		handle->change_state = false;
		/* see if any action is necessary */
		if (handle->curr_state == handle->next_state)
			return;

#if DEBUG_STATE
		printf("%s -> %s\r\n",
		state_strings[handle->next_state],
		state_strings[handle->curr_state]);
#endif

		/* call state exit function */
		handle->from_state_table[handle->curr_state](handle->next_state, handle->flags);

		/* call state entrance function */
		handle->to_state_table[handle->next_state](handle->curr_state, handle->flags);

		/* update state information */
		handle->prev_state = handle->curr_state;
		handle->curr_state = handle->next_state;

		SET_TIMESTAMP(handle->curr_state); // Update when we entered this state	
	}
}

const char *state_strings[] = {
	"PRE_RUN_FAULT", "RUN_FAULT", "POST_RUN_FAULT", "IDLE", "READY_FOR_PUMPDOWN", "PUMPDOWN", "READY", "PROPULSION_START", "PROPULSION_DISTANCE", "BRAKING", "POST_RUN", "SAFE_TO_APPROACH", "SERVICE_LOW_SPEED_PROPULSION"
};
