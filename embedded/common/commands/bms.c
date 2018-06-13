#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "bms.h"

command_status do_bms(int argc, char *argv[]) {
        printf("\r\n%s: TODO (%d args given)\r\n", argv[0], argc);
	
	//BMS Queries
	
	/* Description 			Unit 	PID	Length (bytes)
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
	 */ 
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
	





	printf("\r\nargs: ");
	for(int i = 1; i < argc; i++){
		printf("%s, ", argv[i]);
	}
	
	
	if (!strcmp("query", argv[1])){
		printf("in query\r\n");
		if (!strcmp("volt", argv[2])){
		        printf("in volt\r\n");
			if (!strcmp("pack", argv[3])){
		        	printf("in pack\r\n");
				bms_getPackVolt();
			} else if (!strcmp("pack_open_circuit", argv[3])){
				bms_getPackVoltOpenCircuit();
			} else if (!strcmp("cell_high", argv[3])){
			        bms_getPackVoltHigh();
			} else if (!strcmp("cell_high_num", argv[3])){
			        bms_getPackVoltHighNum();
			} else if (!strcmp("cell_low", argv[3])){
			        bms_getPackVoltLow();
			} else if (!strcmp("cell_low_num", argv[3])){
				bms_getPackVoltLowNum();
			} else if (!strcmp("cell_avg", argv[3])){
			        bms_getPackVoltCellAvg();
			} else if (!strcmp("all", argv[3])){
			        bms_getCellVoltAll();
			}
		} else if (!strcmp("res", argv[2])){
			if (!strcmp("pack", argv[3])){
				bms_getPackRes();
			} else if (!strcmp("cell_high", argv[3])){
				bms_getCellResHigh();
			} else if (!strcmp("cell_high_num", argv[3])){
			        bms_getCellResHighNum();
			} else if (!strcmp("cell_low", argv[3])){
			        bms_getCellResLow();
			} else if (!strcmp("cell_low_num", argv[3])){
			        bms_getCellResLowNum();
			} else if (!strcmp("cell_avg", argv[3])){
			        bms_getCellResAvg();
			} else if (!strcmp("all", argv[3])){
			        bms_getCellResAll();
			}
		} else if (!strcmp("temp", argv[2])){
		        if (!strcmp("cell_high", argv[3])){
		                bms_getCellTempHigh(); 
			} else if (!strcmp("cell_low", argv[3])){
			        bms_getCellTempLow();
			} else if (!strcmp("cell_avg", argv[3])){
			        bms_getCellTempAvg();
			}
		} else if (!strcmp("gen", argv[2])){
		        if(!strcmp("relays", argv[3])){
				bms_getRelayStatus();
			} else if (!strcmp("cell_count", argv[3])){
			        bms_getCellCount();
			} else if (!strcmp("charge_curr_limit", argv[3])){
			        bms_getChargeCurrLimit();
			} else if (!strcmp("discharge_curr_limit", argv[3])){
			        bms_getDischargeCurrLimit();
			} else if (!strcmp("current",argv[3])){
			        bms_getPackCurrent();
			} else if (!strcmp("state_of_charge", argv[3])){
			        bms_getStateOfCharge();
			} else if (!strcmp("amp_hrs", argv[3])){
			        bms_getAmpHours();
			} else if (!strcmp("depth_of_discharge", argv[3])){
			        bms_getDepthOfDischarge();
			} else if (!strcmp("pack_health", argv[3])){
			        bms_getPackHealth();
			}
		}

        } else if(!strcmp("reset", argv[1])){
	        printf("Function not supported by Orion BMS");
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
