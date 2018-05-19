#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "bms.h"

command_status do_bms(int argc, char *argv[]) {
        printf("%s: TODO (%d args given)\r\n", argv[0], argc);
	
	/*BMS Queries*/
	/*
	 * Description 			Unit 	PID	Length (bytes)
	 * Pack State
	 * Relays Status			0xF004	2
	 * Populated Cell Count			0xF007  1
	 * Pack Charge Current Limit	Amp	0xF00A  2
	 * Pack Discharge Current Limit	Amp	0xF00B  2
	 * Signed Pack Current 		Amp	0xF00C  2 NOTE: Subtrack 32767 to get actual amperage
	 * Pack Voltage			V	0xF00D  2
	 * Pack Open Voltage		V	0xF00E  2
	 * Pack State of Charge		%	0xF00F  1
	 * Pack Amphours		Ah	0xF010  2
	 * Pack Resistance 		mOhm	0xF011  2
	 * Pack Depth of Discharge 	%	0xF012  1
	 * Pack Health			%	0xF013  1
	 * Pack Summed Voltage		V	0xF014  2 
	 *
	 * Temperature
	 * Highest Pack Temp		C	0xF028	1
	 * Lowest Pack Temp		C	0xF029  1
	 * Avg. Pack Temp		C	0xF02A  1 
	 * Heatsink Temp Sensor		C	0xF2D	1
	 *
	 * Overall Cell info
	 * Low cell voltage		V	0xF032  2
	 * Low Cell voltage (Num)	#	0xF03E  2
	 * High Cell voltage		V	0xF033  2
	 * High Cell voltage (Num)	#       0xF03D	2
	 * Avg Cell voltage 		V	0xF034  2 
	 * Low Cell resistance 		mOhm	0xF038  2
	 * Low Cell resistance (Num)	#	0XF042	2
	 * High Cell resistance		mOhm	0xF039  2
	 * High cell resistance (Num)	#	0xF041  2
	 * Avg cell resistance 		mOhm	0xF03A  2
	 * Input power supply voltage	V	0xF046	2
	 *
	 * Individual Cell info
	 * Cell voltages (Cells 1-12)	V	0xF100 	24
	 * Cell voltages (Cells 13-24)  V 	0xF101	24 NOTE: Each message includes 12 voltages
	 * ...					...        (each are 2 bytes long)
	 * Cell voltages (Cells 61-72)  V	0xF105	24
	 *
	 * Opencell Volt (Cells 1-12)	V	0xF300  24
	 * Opencell Volt (Cells 13-24)	V	0XF301	24 NOTE: Each message includes 12 voltages
	 * ...					...	   (each are 2 bytes long)
	 * Opencell Volt (Cells 61-72)	V	0xF305  24
	 *
	 * Internal Ohms (Cells 1-12)  	mOhms	0xF200	24 NOTE: Bit 16 (the MSB) indicates whether the
	 * Internal Ohms (Cells 13-24)	mOhms	0xF201	24 cell is actively balancing (1 = balancing, 0 =
	 * ...					...	   not balancing)
	 * Internal Ohms (Cells 61-72) 	mOhms 	0xF205  24
	 *
	 *
	 * */

	printf("\r\nARGV[1]: %s", argv[1]);
        if(!strcmp("query", argv[1])){
	        enum BMS_QUERY query;
		query = RELAY_STATUS;
		printf("\r\n COMMAND PID: %04x", query);
		bms_getRelayStatus();
		return SUCCESS;
	
	} else if(!strcmp("reset", argv[1])){
		return SUCCESS;
	} 
	return SUCCESS;
}

COMMAND_ENTRY(
	"bms",
	"bms { command | query}",
	"Send commands and query BMS data",
	do_bms
)
