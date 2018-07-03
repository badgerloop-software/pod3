#include <stdio.h>
#include <string.h>
#include "dashboard_control.h"
#include "console.h"
#include "usart.h"

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
	}
	else if (!strncmp(state, "ready_for_pumpdown", 18)) {
	        puts("ready_for_pumpdown requested");
	}
	else if (!strncmp(state, "pumpdown", 8)) {
	        puts("pumpdown requested");
	}
	else if (!strncmp(state, "ready", 5)) {
	        puts("ready requested");
	}
	else if (!strncmp(state, "postrun", 7)) {
	        puts("postrun requested");
	}
	else if (!strncmp(state, "service_low_speed", 17)) {
	        puts("service_low_speed requested");
	}
	else if (!strncmp(state, "safe_to_approach", 16)) {
	        puts("safe_to_approach requested");
	}
	else if (!strncmp(state, "prop_start_hyperloop", 20)) {
	        puts("prop_start_hyperloop requested");
	}
	else if (!strncmp(state, "prop_start_openair", 18)) {
	        puts("prop_start_openair requested");
	}
	else if (!strncmp(state, "prop_start_extsub", 17)) {
	        puts("prop_start_extsub requested");
	}
	else if (!strncmp(state, "prop_dsa_hyperloop", 18)) {
	        puts("prop_dsa_hyperloop requested");
	}
	else if (!strncmp(state, "prop_dsa_openair", 16)) {
	        puts("prop_dsa_openair requested");
	}
	else if (!strncmp(state, "prop_dsa_extsub", 15)) {
	        puts("prop_dsa_extsub requested");
	}
	else if (!strncmp(state, "braking_hyperloop", 17)) {
	        process_input("uart send\n\r");
	        puts("braking_hyperloop requested\n\r");
	}
	else if (!strncmp(state, "braking_openair", 15)) {
	        puts("braking_openair requested");
	}
	else if (!strncmp(state, "braking_extsub", 14)) {
	        puts("braking_extsub requested");
	}
	else if (!strncmp(state, "fault_prerun", 12)) {
	        puts("fault_prerun requested");
	}
	else if (!strncmp(state, "fault_run", 9)) {
	        puts("fault_run requested");
	}
	else if (!strncmp(state, "fault_postrun", 13)) {
	        puts("fault_postrun requested");
	}
}

void process_manual_override(char *override) {
        printf("Hit process_manual_override with override '%s'\n", override);
	if (!strncmp(override, "hv_enable", 9)) {
       	        /* pretend we executed the command from the console */
		process_input("can hv_enable");
	}
       	else if (!strncmp(override, "hv_disable", 10)) {
       	        /* pretend we executed the command from the console */
		process_input("can hv_disable");
	}
       	else if (!strncmp(override, "prim_brake_on", 13)) {
		
	}
	else if (!strncmp(override, "prim_brake_off", 14)) {

	}
	else if (!strncmp(override, "prim_brake_vent_on", 18)) {

	}
	else if (!strncmp(override, "prim_brake_vent_off", 19)) {

	}
		
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
