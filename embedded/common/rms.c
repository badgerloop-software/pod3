#include <string.h>
#include <stdio.h>
#include "rms.h"
#include "commands.h"
#include "can.h"
uint8_t data[8];
uint64_t length = 8;

/*Initialization function*/
/* TODO define command byte info macros
 * void rms_init_run(void){
 * char* can_bytes;
 * rms_clearFaults(CFMACRO);
 * rms_enableHeartbeat(EHEARTMACRO);
 * rms_multDisableInverter(DINVERTERMACRO);
 * can_bytes = can_query(0x0AA);
 if ( (can_bytes[6] & 0x80) != 0 ){
 * printf("\r\nrms ready to enable\r\n");
 * }
 * }*/ 
/*
char* rms_getState(void){ 
       printf("\r\nGetState\r\n"); 
       uint16_t can_id = (uint16_t) 0x0AA;
       uint64_t length = (uint64_t) 8;
       
       return can_read(0x0AA); 
} 

uint8_t rms_getLockoutEnabled(void){ 
       printf("\r\nGetLockoutEnabled\r\n"); 
       return (uint8_t) *can_query(0x0AA); 
}
char* rms_getFaults(void){ 
       printf("\r\nGetFaults\r\n"); 
       return can_query(0x0AB); 
}

int32_t rms_getLVVolt(void){ 
       printf("\r\nGetLVVolt\r\n"); 
       return (int32_t) *can_query(0x0A9); 
}

int32_t rms_getHVVolt(void){ 
       printf("\r\nGetHVVolt\r\n"); 
       return (int32_t) *can_query(0x0A7); 
}

uint16_t rms_checkFaults(void){ 
       printf("\r\nCheckFaults\r\n"); 
       return (uint16_t) *can_query(0x0C2); 
}

int32_t rms_getSpeed(void){ 
       printf("\r\nGetSpeed\r\n"); 
       return (int32_t) *can_query(0x0A5); 
}

int32_t rms_getCurrent(void){ 
       printf("\r\nGetCurrent\r\n"); 
       return (int32_t) *can_query(0x0A6); 
}

int32_t rms_getTorque(void){ 
       printf("\r\nGetTorque\r\n"); 
       return (int32_t) *can_query(0x0AC); 
}

int32_t rms_getRPM(void){ 
       printf("\r\nGet\r\n"); 
       return (int32_t) *can_query(0x0A5); 
}
*/
int rms_clearFaults(){ 
	printf("\r\nClearFaults\r\n");
       	uint16_t can_id = 0x00c1;	
		       
	data[0] = 0x14;
  	data[1] = 0x0;
	data[2] = 0x1;
	//data[3] = XX
	data[4] = 0x0;
	data[5] = 0x0;
	//data[6] = XX;
	//data[7] = XX;
	
        can_send(can_id, length, data, &hcan); 
       	return 0;	
}

int rms_enableHeartbeat(){ 
	printf("\r\nEnableHeartbeat\r\n"); 
	uint16_t can_id = 0x00c1;	
	              		
	data[0] = 0x92;
	data[1] = 0x0;
	data[2] = 0x1;	
	//data[3] = XX;
	data[4] = 0x1;
	data[5] = 0x0;
 	//data[6] = XX;
	//data[7] = XX;
	
	can_send(can_id, length, data, &hcan); 
	return 0;
}

int rms_disableInverter(){ 
        printf("\r\ndisableInverter\r\n"); 
	         										                           	              
	uint16_t can_id = 0x00c1;	
	
    	data[0] = 0x0;
        data[1] = 0x0;
        data[2] = 0x0;
        data[3] = 0x0;
        data[4] = 0x0;
        data[5] = 0x0;
        data[6] = 0x0;
        data[7] = 0x0;   									       
               
	can_send(can_id, length, data, &hcan); 
	return 0;
}

int rms_enableInverter(){ 
	printf("\r\nEnableInverter\r\n"); 
	uint16_t can_id = 0x00c0;	
	data[0] = 0x0;
	data[1] = 0x0;
	data[2] = 0x0;
	data[3] = 0x0;
	data[4] = 0x1;
	data[5] = 0x1;
	data[6] = 0x0;
	data[7] = 0x0;
	can_send(can_id, length, data, &hcan); 

	return 0;
} 

int rms_setTorque(uint8_t val){ 
	printf("\r\nSetTorque\r\n"); 
	uint16_t can_id = 0x00c0;	
	if(val == 10){
		data[0] = 0x64;
		data[1] = 0x0;
	} else if (val == 20){
		data[0] = 0xC8;
		data[1] = 0x0;
	} else if (val == 30){
		data[0] = 0x2C;
		data[1] = 0x1;
	}
	//data[2] = XX;
	//data[3] = XX
	data[4] = 0x1;
	data[5] = 0x1;
	data[6] = 0x0;
	data[7] = 0x0;

	can_send(can_id, length, data, &hcan); 

	return 0;

}

int rms_multDisableInverter(){ 
	int i;
	printf("\r\nMultDisableInverter\r\n");

	for(i = 0; i < 5; i++)
		rms_disableInverter();
	return 0;
} 

int rms_rpmThresDisable(){ 
	printf("\r\nRPMThresDisable\r\n"); 


	uint16_t can_id = 0x00c0;	

	//data[0] = XX;
	//data[1] = 0;
	//data[2] = 1;
	//data[3] = XX
	data[4] = 0x1;
	data[5] = 0x2;
	data[6] = 0x0;
	data[7] = 0x0;


	can_send(can_id, length, data, &hcan); 
	return 0;
}
