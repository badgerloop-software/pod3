#include <string.h>
#include <stdio.h>
#include "bms.h"
#include "can.h"
#include <stm32l4xx_hal.h>

uint8_t bms_getRelayStatus(void){ 
        printf("\r\nGetRelayStatus\r\n"); 

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf004;
	
	uint8_t * return_data = can_send_obd2(can_id, length, mode, pid, &hcan);

	//maximum value: 0xffff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
} 
 

uint8_t bms_getCellCount(void){


	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf007;
	uint8_t * return_data = can_send_obd2(can_id, length, mode, pid,  &hcan);

	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}

uint8_t bms_getPackVolt(){
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00d;
	uint8_t * return_data = can_send_obd2(can_id, length, mode, pid, &hcan);
	
	//maximum value: 0xfff
	//minimum value: 0
	//scaling: 0.1

	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);
	return scaled_value;


}

uint8_t bms_getPackVoltOpenCircuit(void){

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00e;
	uint8_t * return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}
uint8_t bms_getPackVoltHigh(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf033;
	uint8_t * return_data = can_send_obd2(can_id, length, mode, pid,  &hcan);
	

	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getPackVoltHighNum(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf03d;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t  bms_getPackVoltLow(void){

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf032;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getPackVoltLowNum(void){
	
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf03e;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);
	

	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getPackVoltCellAvg(void){
	

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf034;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);

	//maximum value: 0xfff
	//minimum value: 0

	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getCellVoltAll(void){
        //TODO loop through all cell info
        //bms_can_send( );
        return 0;
}  

uint8_t bms_getPackRes(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf011;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);
	

	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}

uint8_t bms_getCellResHigh(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf039;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}

uint8_t bms_getCellResHighNum(void){

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf041;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}
uint8_t bms_getCellResLow(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf038;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getCellResLowNum(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf042;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getCellResAvg(void){
	

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf03a;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getCellResAll(void){
	//TODO loop through cell info
	//bms_can_send( );
	return 0;
}
uint8_t bms_getCellTempHigh(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf028;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}
uint8_t bms_getCellTempLow(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf029;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}
uint8_t bms_getCellTempAvg(void){

	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf02a;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 
}
uint8_t bms_getCellTempAll(void){
        //TODO
        //bms_can_send( );
	return 0;
}
uint8_t bms_getRelayState(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf004;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getChargeCurrLimit(void){
	
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00a;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid,  &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getDischargeCurrLimit(void){

	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00b;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getPackCurrent(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00c;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getStateOfCharge(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf00f;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getAmpHours(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 2;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf010;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
uint8_t bms_getDepthOfDischarge(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf012;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid, &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}

uint8_t bms_getPackHealth(void){
        
	uint16_t can_id = 0x7df;
	size_t length = 1;
	uint8_t mode = 0x22;
	uint16_t pid = 0xf013;
	uint8_t *return_data = can_send_obd2(can_id, length, mode, pid,  &hcan);


	//maximum value: 0xfff
	//minimum value: 0
	//scaling: none
	uint8_t scaled_value = return_data[0] + (return_data[1] << 8);

	return scaled_value; 

}
