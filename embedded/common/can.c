#include <string.h>
#include <stdio.h>
#include "can.h"

char* can_read(){
	printf("\r\nCAN READ\\");
	return "TEST";
}


void can_send(uint16_t can_id, uint64_t length, uint64_t data){
	printf("\r\nCAN SEND ID: %d length: %llu data: %llu\r\n", can_id, length, data);
	
}
