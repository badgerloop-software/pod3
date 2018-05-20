#include <string.h>
#include <stdio.h>
#include "bms.h"

/* temporary CAN message send */
char* can_query(uint16_t pid){ 
        printf("\r\n\nSending CAN message %04x\r\n", pid); 
        return "sucessful CAN Query"; 
} 
 
char* bms_getRelayStatus(void){ 
        printf("\r\nGetRelayStatus\r\n"); 
        return can_query(0xF004); 
} 
 

int bms_getCellCount(void){
	return (int) can_query(0xF007);
}


char* bms_can_send(uint64_t address){
        /* placeholder use of byte_info */
	printf("\r\nSending CAN Message to Address: %llu and message: \r\n", address);
	char* query_returned = "CAN VALUE";
	return query_returned;
}
double bms_getPackVoltOpenCircuit(void){
	bms_can_send(0xF00E     );
	return 0;
}
double bms_getPackVoltHigh(void){
	bms_can_send(0xF033     );
	return 0;

}
uint8_t bms_getPackVoltHighNum(void){
	bms_can_send(0xF03D     );
	return 0;

}
double  bms_getPackVoltLow(void){
	bms_can_send(0xF032     );
	return 0;

}
int bms_getPackVoltLowNum(void){
	bms_can_send(0xF03E     );
	return 0;

}
double bms_getPackVoltCellAvg(void){
	bms_can_send(0xF034     );
	return 0;

}
double bms_getCellVoltAll(void){
        //TODO loop through all cell info
        //bms_can_send( );
        return 0;
	}  
double bms_getPackRes(void){
	bms_can_send(0xF011     );
        return 0;
	}
double bms_getCellResHigh(void){
        bms_can_send(0xF039     );
        return 0;
        }
int bms_getCellResHighNum(void){
        bms_can_send(0xF041     );
	return 0;
}
double bms_getCellResLow(void){
	bms_can_send(0xF038     );
	return 0;

}
int bms_getCellResLowNum(void){
	bms_can_send(0xF042     );
	return 0;

}
double bms_getCellResAvg(void){
	bms_can_send(0xF03A     );
	return 0;

}
double bms_getCellResAll(void){
	//TODO loop through cell info
	//bms_can_send( );
	return 0;
	}
double bms_getCellTempHigh(void){
        bms_can_send(0xF028);
        return 0;
        }
double bms_getCellTempLow(void){
        bms_can_send(0xF029     );
        return 0;
        }
double bms_getCellTempAvg(void){
	bms_can_send(0xF02A     );
	return 0;
	}
double bms_getCellTempAll(void){
        //TODO
        //bms_can_send( );
	return 0;
}
char*  bms_getRelayState(void){
        bms_can_send(0xF004     );
        return "0";

}
double bms_getChargeCurrLimit(void){
	bms_can_send(0xF00A     );
	return 0;

}
double bms_getDischargeCurrLimit(void){
	bms_can_send(0xF00B     );
	return 0;

}
double bms_getPackCurrent(void){
        bms_can_send(0xF00C     );
        return 0;

}
double bms_getStateOfCharge(void){
        bms_can_send(0xF00F     );
        return 0;

}
double bms_getAmpHours(void){
        bms_can_send(0xF010     );
        return 0;

}
double bms_getDepthOfDischarge(void){
        bms_can_send(0xF012     );
        return 0;

}

double bms_getPackHealth(void){
        bms_can_send(0xF013     );
        return 0;

}
