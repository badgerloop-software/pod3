#include <stdio.h>
#include "state_machine.h"
#include "state_handlers.h"
#include "system.h"
#include "board.h"
#include "badgerloop.h"
#include "can.h"

//TODO: Some states cannot be transitioned out of by timer
const unsigned int state_intervals[] = {
	999999,	/* PRE_RUN_FAULT			*/
	999999,	/* RUN_FAULT 				*/
	999999,	/* POST_RUN_FAULT 			*/
	999999,	/* IDLE  				*/
	999999,	/* READY_FOR_PUMPDOWN 			*/
	10000,	/* PUMPDOWN 				*/
	999999,	/* READY				*/
	10000,	/* PROPULSION_START 			*/
	10000,	/* PROPULSION_DISTANCE 			*/
	10000,	/* BRAKING				*/
	10000,	/* POST_RUN 				*/
	1000,	/* SAFE_TO_APPROACH			*/
	999999, /* SERVICE_LOW_SPEED_PROPULSION		*/
};

void change_state(STATE_NAME state) {
	if(state > NUM_STATES) return;
	printf("Changing state from %s to %s\r\n", state_strings[state_handle.curr_state],
		state_strings[state]);
	state_handle.next_state = state;
	state_handle.change_state = true;
}

void assert_pre_run_fault(const char *message) {
	change_state(PRE_RUN_FAULT);
	printf("%s: %s\r\n", __func__, message);
	fault_message = message;
}

void assert_run_fault(const char *message) {
	change_state(RUN_FAULT);
	printf("%s: %s\r\n", __func__, message);
	fault_message = message;
}

void assert_post_run_fault(const char *message) {
	change_state(POST_RUN_FAULT);
	printf("%s: %s\r\n", __func__, message);
	fault_message = message;
}

void clear_flag(uint32_t flags) {
	state_handle.flags &= ~flags;
}

void set_flag(uint32_t flags) {
	state_handle.flags |= flags;
}

void set_propulsion_timeout(uint32_t timeout){
	propulsion_timeout_ms = timeout;
}
/*****************************************************************************/
/*                              Pre_Run_Fault Handlers                        */
/*****************************************************************************/
void to_pre_run_fault(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 2, Solenoid 3;  both systems released w/ backup
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to max 0.
    if(board_type==PV) {
        /* De-assert high voltage and RMS */
        mcu_high_voltage_set(false);
        pv_solenoid2_set(false);
    } // end PV_MODULE

    if(board_type==NAV) {

    }//end NAV module
    if(board_type==DASH) {
        can_heartbeat_fault();
        can_heartbeat_handler( &can_handle );
    }
    if(board_type==DEV) {

    } // end DEV_MODULE

    if(board_type==DASH) {
        can_heartbeat_fault();
        //can_heartbeat_handler( &can_handle );
    } // end CPP_MODULE

if(board_type==DEV) {

} // end DEV_MODULE

	printf("To state: PRE_RUN_FAULT (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_pre_run_fault(uint32_t flags) {
	// Check all sensors are within range
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	UNUSED( flags );
	//printf("In state: PRE_RUN_FAULT (Flags: 0x%lx)\r\n", flags);
}

void from_pre_run_fault(STATE_NAME to, uint32_t flags) {
	printf("From state: PRE_RUN_FAULT (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                              Run_Fault Handlers                        */
/*****************************************************************************/
void to_run_fault(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 3,4,5;  both systems clamped
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to 0
	if(board_type==PV) {
        /* De-assert high voltage and RMS */
        mcu_high_voltage_set(false);
        pv_solenoid2_set(false);

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_fault();
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: RUN_FAULT (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_run_fault(uint32_t flags) {
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	UNUSED( flags );
	//printf("In state: RUN_FAULT (Flags: 0x%lx)\r\n", flags);

}

void from_run_fault(STATE_NAME to, uint32_t flags) {
	printf("From state: RUN_FAULT (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                              Post_Run_Fault Handlers                        */
/*****************************************************************************/
void to_post_run_fault(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// No solenoids powered; primary clamped and venting
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to 0
	if(board_type==PV) {
        /* De-assert high voltage and RMS */
        mcu_high_voltage_set(false);
        pv_solenoid2_set(false);

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_fault();
        //can_heartbeat_handler();

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: POST_RUN_FAULT (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_post_run_fault(uint32_t flags) {
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	UNUSED( flags );
	//printf("In state: POST_RUN_FAULT (Flags: 0x%lx)\r\n", flags);
}

void from_post_run_fault(STATE_NAME to, uint32_t flags) {
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
	
	} // end DEV_MODULE
	printf("From state: POST_RUN_FAULT (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                 Idle Handlers                             */
/*****************************************************************************/
void to_idle(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 3; This has both systems released
	/*****************************/
	/*       Propulsion          */
	/*****************************/
  // Set torque/speed to 0

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: IDLE (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_idle(uint32_t flags) {
	UNUSED( flags );
	//printf("In state: IDLE (Flags: 0x%lx)\r\n", flags);
	// Pod health check
if(board_type==PV) {
    /* BMS Reset gets asserted */
    bms_software_reset_set( true );

} // end PV_MODULE

if(board_type==NAV) {
	//int pres = GET_PRES_TANK_PRI, bUpper = BRAKING_TANK_UPPER;
	//int bLower = BRAKING_TANK_LOWER;
	//if(pres > bUpper){} //TODO Throw warning
	//if(pres < bLower) 
	//	assert_pre_run_fault("Primary braking tank underpressure\r\n");
	//pres = GET_PRES_TANK_SEC;
	//if(pres>bUpper){}
	//if(pres<bLower) 
	//	assert_pre_run_fault("Secondary braking tank underpressure\r\n");
//	pres = GET_PRES_LINE_PRI; bUpper = BRAKING_LINE__UPPER;
//	bLower = BRAKING_LINE__LOWER

    } // end NAV_MODULE 
    if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );
		
    } // end CPP_MODULE

    if(board_type==DEV) {

    } // end DEV_MODULE
}

void from_idle(STATE_NAME to, uint32_t flags){
	if(board_type==PV) {
	    
        /* BMS reset de-asserted */
        bms_software_reset_set( false );

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
    
    printf("From state: IDLE (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                 Ready_For_Pumpdown Handlers               */
/*****************************************************************************/
void to_ready_for_pumpdown(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 3; This has both systems released
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque/speed to 0

	// Perform braking checklist:
	//	1. Unpower solenoid 3; check for 15 psi decrease in system 1 in TODO sec.
	// 	2. Power sol. 3,4,5; check for pressure sensor 5 > 120PSI < 166psi
	//		 	check secondary actuation limit switch
	//	3. Unpower sol. 4,5; check secondary actuation limit switch
	//			check pressure sensor 5 < 50psi
	//	4. Unpower sol. 1; check pressure sensor 3 >120 psi, <166psi
	//		  check for primary limit switch actuation
	// 			get system pressure after 1 sec.
	//			check for 15 psi decrease in TODO sec.
	// 	5. Power sol. 1; Braking checklist complete

	if(board_type==PV) {

        /* Enabling high voltage, enabling RMS */
        mcu_high_voltage_set(true);
        pv_solenoid2_set(true);

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_next();
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
	
	printf("To state: READY_FOR_PUMPDOWN (From: %s Flags: 0x%lx)\r\n", 
			state_strings[from], flags);
}

void in_ready_for_pumpdown(uint32_t flags) {
	//printf("In state: READY_FOR_PUMPDOWN (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_ready_for_pumpdown(STATE_NAME to, uint32_t flags){
	printf("From state: READY_FOR_PUMPDOWN (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Pumpdown Handlers                          */
/*****************************************************************************/
void to_pumpdown(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 3; both released
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque/speed to 0

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: PUMPDOWN (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_pumpdown(uint32_t flags) {
	//printf("In state: PUMPDOWN (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_pumpdown(STATE_NAME to, uint32_t flags){
	printf("From state: PUMPDOWN (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}


/*****************************************************************************/
/*                                Ready Handlers                             */
/*****************************************************************************/
void to_ready(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 2, Solenoid 3; both released w/ backup (2 fault)
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque/speed to 0

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
	printf("To state: READY (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_ready(uint32_t flags) {
	//printf("In state: READY (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_ready(STATE_NAME to, uint32_t flags){
	printf("From state: READY (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}


/*****************************************************************************/
/*                                Propulsion_Start Handlers                  */
/*****************************************************************************/
uint32_t propulsion_start_ts;
void to_propulsion_start(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 2, Solenoid 3;  both systems released w/ backup
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to max torque.

	if(board_type==PV) {

	} // end PV_MODULE

	else if(board_type==NAV) {

	} // end NAV_MODULE

	else if(board_type==DASH) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );
	    	propulsion_start_ts = ticks;
	} // end CPP_MODULE

	else if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
	printf("To state: PROPULSION_START (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_propulsion_start(uint32_t flags) {
	//printf("In state: PROPULSION_START (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );
	    
        /* If we pass propulsion timeout enter braking 
	    if (ticks >= propulsion_start_ts + propulsion_timeout_ms){
		    change_state(BRAKING);
	    } */
	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_propulsion_start(STATE_NAME to, uint32_t flags){
	printf("From state: PROPULSION_START (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Propulsion_Distance Handlers                  */
/*****************************************************************************/
void to_propulsion_distance(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 2, Solenoid 3;  both systems released w/ backup
	// Begin checking distance method?????

	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to max torque.

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: PROPULSION_DISTANCE (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_propulsion_distance(uint32_t flags) {
	//printf("In state: PROPULSION_DISTANCE (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {
	
	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );
	
        /* If we pass propulsion timeout enter braking 
	    if (ticks >= propulsion_start_ts + propulsion_timeout_ms){
		    change_state(BRAKING);
	    } */
	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_propulsion_distance(STATE_NAME to, uint32_t flags){
	printf("From state: PROPULSION_DISTANCE (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Braking Handlers                           */
/*****************************************************************************/
void to_braking(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Solenoid 3; Stop secondary venting, actuate primary
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to 0
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
	printf("To state: BRAKING (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_braking(uint32_t flags) {
	//printf("In state: BRAKING (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_braking(STATE_NAME to, uint32_t flags){
	printf("From state: BRAKING (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Post_Run Handlers                           */
/*****************************************************************************/
void to_post_run(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// NO solenoids powered; THIS VENTS BOTH SYSTEMS. Primary will actuate if air
	// If the desire is to use system again, go to idle, not post_run
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque/speed to 0
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
	printf("To state: POST_RUN (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_post_run(uint32_t flags) {
	//printf("In state: POST_RUN (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        //Heartbeat next does not advance past post-run
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {
        	can_heartbeat_next();
        	can_heartbeat_handler( &can_handle );

	} // end DEV_MODULE
}

void from_post_run(STATE_NAME to, uint32_t flags){
	printf("From state: POST_RUN (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Safe_To_Approach Handlers                  */
/*****************************************************************************/
void to_safe_to_approach(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
  // No solenoids powered
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set torque to 0

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: SAFE_TO_APPROACH (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_safe_to_approach(uint32_t flags) {
	//printf("In state: SAFE_TO_APPROACH (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// Pod health check
	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_safe_to_approach(STATE_NAME to, uint32_t flags){
	printf("From state: SAFE_TO_APPROACH (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/*****************************************************************************/
/*                                Service_Low_Speed_Propulsion Handlers      */
/*****************************************************************************/
void to_service_low_speed_propulsion(STATE_NAME from, uint32_t flags) {
	/*****************************/
	/*       Braking             */
	/*****************************/
	// Power Solenoid 1, Solenoid 2, Solenoid 3;  both systems released w/ backup
	/*****************************/
	/*       Propulsion          */
	/*****************************/
	// Set speed to low speed (no more than 5MPH)

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
	printf("To state: LOW_SPEED_PROPULSION (From: %s Flags: 0x%lx)\r\n", state_strings[from], flags);
}

void in_service_low_speed_propulsion(uint32_t flags) {
	//printf("In state: LOW_SPEED_PROPULSION (Flags: 0x%lx)\r\n", flags);
	UNUSED( flags );
	// pod health check

	if(board_type==PV) {

	} // end PV_MODULE

	if(board_type==NAV) {

	} // end NAV_MODULE

	if(board_type==DASH) {
        can_heartbeat_handler( &can_handle );

	} // end CPP_MODULE

	if(board_type==DEV) {

	} // end DEV_MODULE
}

void from_service_low_speed_propulsion(STATE_NAME to, uint32_t flags){
	printf("From state: LOW_SPEED_PROPULSION (To: %s Flags: 0x%lx)\r\n", state_strings[to], flags);
}

/****************************************************************************/

unsigned int state_event_timestamps[NUM_STATES];

state_transition_t *to_handlers[] = {
	to_pre_run_fault, to_run_fault, to_post_run_fault, to_idle, to_ready_for_pumpdown, to_pumpdown, to_ready, to_propulsion_start, to_propulsion_distance, to_braking, to_post_run, to_safe_to_approach, to_service_low_speed_propulsion
};

state_handler_t *in_handlers[] = {
	in_pre_run_fault, in_run_fault, in_post_run_fault, in_idle, in_ready_for_pumpdown, in_pumpdown, in_ready, in_propulsion_start, in_propulsion_distance, in_braking, in_post_run, in_safe_to_approach, in_service_low_speed_propulsion
};

state_transition_t *from_handlers[] = {
	from_pre_run_fault, from_run_fault, from_post_run_fault, from_idle, from_ready_for_pumpdown, from_pumpdown, from_ready, from_propulsion_start, from_propulsion_distance, from_braking, from_post_run, from_safe_to_approach, from_service_low_speed_propulsion
};;
