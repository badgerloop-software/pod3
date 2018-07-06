#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "rms.h"
#include <stdlib.h>

command_status do_rms(int argc, char *argv[]) {
	printf("%s: rms (%d args given)\r\n", argv[0], argc);
	/*RMS Commands*/
	/*Usage: rms command <name> <val>*/
	/*  
  	 * name			val	CAN ID       Description
 	 * clear_faults			(0x0C1)      Clear Faults (Must be done before Enable CAN Heartbeat message or timeout will occur)
 	 * enable_heartbeat		(0x0C1)      Enable CAN Heartbeat Message
 	 * disable_inverter		(0x0C0)      Send out inverter disable command
 	 * enable_inverter		(0x0C0)      Enable inverter forward
 	 * set_torque		(Nm)	(0x0C0)      Set Torque level
 	 * mult_disable_inverter	(0x0C0)      Disable Inverter (send multiple times)
 	 * rpm_thres_disable 	(v)	(0x0C0)      RPM Threshold disable inverter (Discharge DC Bus)
 	 *
 	 * */ 
 	/* RMS DAQ */
	/* Usage: bms query <name> */
	/* 
	 * name		CAN ID		Description            
	 * state	(0x0AA)      	Internal States (Monitor Torque or Speed Mode)
	 * lockout	(0x0AA)		Torque mode is active, lockout enabled
	 * faults	(0x0AB)      	Monitor Faults
	 * lv_batt	(0x0A9)      	Monitor 12V system voltage
	 * hv_batt	(0x0A7)      	DC Bus Voltage
	 * check_faults	(0x0C2)     	Check faults were cleared
	 * speed	(0x0A5)     	Monitor Speed
	 * current	(0x0A6)      	Monitor Current
 	 * torque	(0x0AC)      	Monitor Torque
 	 * rpm		(????) 		Monitor RPM
         *
	 * */

	/*
	printf("argv[1]: %s argv[2]: %s",argv[1], argv[2]);
	if(!strcmp("query", argv[1])){
		if (!strcmp("state",argv[1])){
			rms_getState();
		} else if (!strcmp("lockout", argv[2])){
			rms_getLockoutEnabled();
		} else if (!strcmp("faults", argv[2])){
			rms_getFaults();
		} else if (!strcmp("lv_batt", argv[2])){
			rms_getLVVolt();
		} else if (!strcmp("hv_batt", argv[2])){
			rms_getHVVolt();
		} else if (!strcmp("check_faults", argv[2])){
			rms_checkFaults();
		} else if (!strcmp("speed", argv[2])){
			rms_getSpeed();
		} else if (!strcmp("current", argv[2])){
			rms_getCurrent();
		} else if (!strcmp("torque", argv[2])){
			rms_getTorque();
		} else if (!strcmp("rpm", argv[2])){
			rms_getRPM();
		}
	} if(!strcmp("command", argv[1])){
		if (!strcmp("clear_faults", argv[2])){
			rms_clearFaults(argv[3]);
		} else if (!strcmp("enable_heartbeat", argv[2])){
			rms_enableHeartbeat(argv[3]);
		} else if (!strcmp("disable_inverter", argv[2])){
			rms_disableInverter(argv[3]);
		} else if (!strcmp("enable_inverter", argv[2])){
			rms_enableInverter(argv[3]);
		} else if (!strcmp("set_torque", argv[2])){
			rms_setTorque( (uint8_t)atoi(argv[3]) );
		} else if (!strcmp("mult_disable_inverter", argv[2])){
			rms_multDisableInverter(argv[3]);
		} else if (!strcmp("rpm_thres_disable", argv[2])){
			rms_rpmThresDisable(argv[3]);
		} 		
	} 
  	*/	
     	return CMD_SUCCESS;
}
COMMAND_ENTRY(
	"rms",
	"rms {command | query}",
	"Send commands and query rms data",
	do_rms
	)
