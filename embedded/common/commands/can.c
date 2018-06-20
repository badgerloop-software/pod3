#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "can.h"
#include <limits.h>

extern uint8_t message_num;

command_status do_can(int argc, char *argv[]) {

	printf("%s:  (%d args given)\r\n", argv[0], argc);
	long long converted = 0;	
	uint8_t data[8];
	char* str;
	
	//Usage: can read
	if(!strcmp("read", argv[1])){	

		printf("\r\ncalling can_read()\r\n");
		can_read(&hcan);

	//Usage: can send <can ID> <data>
	} else if(!strcmp("send", argv[1])){
		
		
		uint32_t can_id = strtol(argv[2], &str, 16);
			if(can_id > 0x7ff){
				printf("Invalid CAN ID.\r\n");
				return ERROR;
			}
			printf( "CAN ID: %lx\r\n", can_id);
			if(converted == LONG_MAX || converted == LONG_MIN ){
				printf("strtoll() error.\r\n" );
			}
			else if( str == argv[2] ){
				printf("No digits were found.\r\n");
			}
			else if( *str != '\0'){
				printf("Extra string passed in.\r\n");
			}


		size_t length = strlen(argv[3]);

		converted = strtoll(argv[3], &str, 16);
		if(converted == LONG_MAX || converted == LONG_MIN ){
			printf("strtoll() error.\r\n" );
		}
		else if( str == argv[3] ){
			printf("No digits were found.\r\n");
		}
		else if( *str != '\0'){
			printf("Extra string passed in.\r\n");
		}
		
		
		data[7] = (converted & 0xff00000000000000) >> 56;
		data[6] = (converted & 0x00ff000000000000) >> 48;
		data[5] = (converted & 0x0000ff0000000000) >> 40;
		data[4] = (converted & 0x000000ff00000000) >> 32;
		data[3] = (converted & 0x00000000ff000000) >> 24;
		data[2] = (converted & 0x0000000000ff0000) >> 16;
		data[1] = (converted & 0x000000000000ff00) >> 8;
		data[0] = (converted & 0x00000000000000ff);

		printf("can_id: %lx, length: %d\r\n", can_id, length);
		for(int i = 0; i < 8; i++){
			printf("Data #%d: %x\r\n", i,data[i]);
		}
		if( length > 0 && length <= 16){
			can_send(can_id, length, data, &hcan);
		}
		else{
			printf("Data must be <= 16 hex values and > 0.");
			return ERROR;
		}
	
	} else if(!strcmp("update", argv[1])){

		can_update(atoi(argv[2]));
		printf( "Message Number: %d\r\n", message_num);
	}	
	return CMD_SUCCESS;
}

COMMAND_ENTRY(
	"can",
	"can send",
	"can read",
	do_can
)
