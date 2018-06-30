
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "can.h"
#include <limits.h>
#include "badgerloop.h"
extern uint8_t message_num;

command_status do_badgerloop(int argc, char *argv[]) {
	printf("%s:  (%d args given)\r\n", argv[0], argc);
	
	if(!strcmp("send", argv[1])){
		uint8_t TxData[8];
		TxData[0] = (uint8_t) 0x01;
		TxData[1] = (uint8_t) 0x01;
		TxData[2] = (uint8_t) 0x01;
		TxData[3] = (uint8_t) 0x01;
		TxData[4] = (uint8_t) 0x01;
		TxData[5] = (uint8_t) 0x01;
		TxData[6] = (uint8_t) 0x01;
		TxData[7] = (uint8_t) 0x01;
		SENDING_MODULE module1 = CCP_SEND;
		RECEIVING_MODULE module2 = NAV_REC;
		MESSAGE_TYPE type = CCP_FAULT;
		printf("MESSAGE TYPE %x", type);
		intermodule_can_message(module1, module2, 0, type, TxData, &hcan); 
	}
	


	return SUCCESS;
}

COMMAND_ENTRY(
	"badgerloop",
	"badgerloop send",
	"badgerloop read",
	do_badgerloop
)
