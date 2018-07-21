#include <stdio.h>
#include <string.h>
#include "dashboard_control.h"
#include "console.h"
#include "usart.h"
#include "state_handlers.h"
#include "nav_data.h"
#include "pv_data.h"
#include "board.h"
#include "badgerloop.h"
extern const int board_type;
extern state_box stateVal;
extern state_box pv_stateVal;
int str_index_of(char *search, char *find) {
	char *start;
	start = strstr(search, find);
	if (start == NULL) {
	        return -1;
	}
	return start - search;
}


void process_state_transition(char *state) {
        /* sweet lord forgive me for the following logic */
	if (!strncmp(state, "poweroff", 8)) {
	        puts("poweroff requested");
	}
	else if (!strncmp(state, "idle", 4)) {
	        puts("idle requested");
		change_state(IDLE);
		
	}
	else if (!strncmp(state, "ready_for_pumpdown", 18)) {
	        puts("ready_for_pumpdown requested");
		change_state(READY_FOR_PUMPDOWN);
	}
	else if (!strncmp(state, "pumpdown", 8)) {
	        puts("pumpdown requested");
		change_state(PUMPDOWN);
	}
	else if (!strncmp(state, "ready", 5)) {
	        puts("ready requested");
		change_state(READY);
	}
	else if (!strncmp(state, "postrun", 7)) {
	        puts("postrun requested");
		change_state(POST_RUN);
	}
	else if (!strncmp(state, "service_low_speed", 17)) {
	        puts("service_low_speed requested");
		change_state(SERVICE_LOW_SPEED_PROPULSION);
	}
	else if (!strncmp(state, "safe_to_approach", 16)) {
	        puts("safe_to_approach requested");
		change_state(SAFE_TO_APPROACH);
	}
	else if (!strncmp(state, "prop_start_hyperloop", 20)) {
	        puts("prop_start_hyperloop requested");
		change_state(PROPULSION_START);
	}
	else if (!strncmp(state, "prop_start_openair", 18)) {
	        puts("prop_start_openair requested");
		change_state(PROPULSION_START);
	}
	else if (!strncmp(state, "prop_start_extsub", 17)) {
	        puts("prop_start_extsub requested");
		change_state(PROPULSION_START);
	}
	else if (!strncmp(state, "prop_dsa_hyperloop", 18)) {
	        puts("prop_dsa_hyperloop requested");
		change_state(PROPULSION_DISTANCE);
	}
	else if (!strncmp(state, "prop_dsa_openair", 16)) {
	        puts("prop_dsa_openair requested");
		change_state(PROPULSION_DISTANCE);
	}
	else if (!strncmp(state, "prop_dsa_extsub", 15)) {
	        puts("prop_dsa_extsub requested");
		change_state(PROPULSION_DISTANCE);
	}
	else if (!strncmp(state, "braking_hyperloop", 17)) {
	        puts("braking_hyperloop requested");
		change_state(BRAKING);
	}
	else if (!strncmp(state, "braking_openair", 15)) {
	        puts("braking_openair requested");
		change_state(BRAKING);
	}
	else if (!strncmp(state, "braking_extsub", 14)) {
	        puts("braking_extsub requested");
		change_state(BRAKING);
	}
	else if (!strncmp(state, "fault_prerun", 12)) {
	        puts("fault_prerun requested");
		change_state(PRE_RUN_FAULT);
	}
	else if (!strncmp(state, "fault_run", 9)) {
	        puts("fault_run requested");
		change_state(RUN_FAULT);
	}
	else if (!strncmp(state, "fault_postrun", 13)) {
	        puts("fault_postrun requested");
		change_state(POST_RUN_FAULT);
	}
}

void process_manual_override(char *override) {
        /* TODO */
        printf("Hit process_manual_override with override '%s'\n", override);
	if (!strncmp(override, "hv_enable", 9)) {
	        puts("hv_enable requested");
			if (board_type == PV) {
				mcu_high_voltage_set(1);
			}
	        
	}
	else if (!strncmp(override, "hv_disable", 10)) {
	        puts("hv_disable requested");
	        if (board_type == PV) {
				mcu_high_voltage_set(0);
			}
	}
	else if (!strncmp(override, "prim_brake_on", 13)) {
	        puts("prim_brake_on requested");
	        actuate_brakes();
    }
	else if (!strncmp(override, "prim_brake_off", 14)) {
	        puts("prim_brake_off requested");
            actuate_sec_brakes();
	}
	else if (!strncmp(override, "prim_brake_vent_on", 18)) {
	        puts("prim_brake_vent_on requested");
	}
	else if (!strncmp(override, "prim_brake_vent_off", 19)) {
	        puts("prim_brake_vent_off requested");
	}
}

void do_pod_heartbeat() {
        /* TODO  we just got a heartbeat from the dashboard*/
}

void process_control(char *buf) {
        int len;
	int type_payload_len;
	int cmd_start;
        char cmd_type;
	char *payload_start;

	len = strlen(buf);
	/* see where the command starts if we have one */
	cmd_start = str_index_of(buf, "$|");
	/* bail if we didn't get a control for some reason */
	if (cmd_start < 0) { return; }
	/* and see if we have any actual command payload */
	/* first character after $| indicates control type */
	type_payload_len = len - (cmd_start + 2);
	/* bail if we don't have a type and follow on data */
	if (type_payload_len < 2) { return; }
        /* deal with each type of control */
	cmd_type = buf[cmd_start + 2];
	payload_start = buf + cmd_start + 3;
	switch(cmd_type) {
                case 't': /* state transition */
		        process_state_transition(payload_start);
		        break;
	        case 'o': /* manual override */
	                process_manual_override(payload_start);
	                break;
	        case 'h': /* heartbeat from dashboard */
	                do_pod_heartbeat();
	                break;
	}
}

char buffer[USART_BUF];
void check_incoming_controls(PC_Buffer *rx) {

	/* only something to do if a message is waiting */
	if (pc_buffer_messageAvailable(rx)) {

		pc_buffer_getMessage(rx, buffer, USART_BUF);

		/* process the command string if it's not empty */
		if (buffer[0] != '\0') process_control(buffer);

	}
}
