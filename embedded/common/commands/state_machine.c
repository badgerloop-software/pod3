#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "state_handlers.h"
#include "state_machine.h"

command_status do_state_machine(int argc, char *argv[]) {
	char* str;

	/* Set state to change next time state_machine_handler called */
	if(!strcmp("change_state", argv[1])){

		if(!strcmp("pre_run_fault", argv[2])){
			change_state(PRE_RUN_FAULT);
		}
		else if(!strcmp("run_fault", argv[2])){
			change_state(RUN_FAULT);
		}
		else if(!strcmp("post_run_fault,", argv[2])){
			change_state(POST_RUN_FAULT);
		}
		else if(!strcmp("idle", argv[2])){
			change_state(IDLE);
		}
		else if(!strcmp("ready_for_pumpdown", argv[2])){
			change_state(READY_FOR_PUMPDOWN);
		}
		else if(!strcmp("pumpdown", argv[2])){
			change_state(PUMPDOWN);
		}
		else if(!strcmp("ready", argv[2])){
			change_state(READY);
		}
		else if(!strcmp("propulsion_start", argv[2])){
			change_state(PROPULSION_START);
		}
		else if(!strcmp("propulsion_distance", argv[2])){
			change_state(PROPULSION_DISTANCE);
		}
		else if(!strcmp("braking", argv[2])){
			change_state(BRAKING);
		}
		else if(!strcmp("post_run", argv[2])){
			change_state(POST_RUN);
		}
		else if(!strcmp("safe_to_approach", argv[2])){
			change_state(SAFE_TO_APPROACH);
		}
		else if(!strcmp("service_low_speed_propulsion", argv[2])){
			change_state(SERVICE_LOW_SPEED_PROPULSION);
		}
		else {
			printf("invalid state");
		}
	}
	
	/* Set propulsion timeout and next state */
	if(!strcmp("set_timeout", argv[1])){
		if (argc == 3) {
			uint32_t timeout_ms = strtoul(argv[2], &str, 10);
		 	set_propulsion_timeout(timeout_ms); 
		} else {
			printf("specify time in ms\r\n");
		}
	}
	
	return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"state_machine",
	"state_machine { change_state | timeout }",
	"Signal a state change to the state machine and set propulsion timeout",
	do_state_machine
)
