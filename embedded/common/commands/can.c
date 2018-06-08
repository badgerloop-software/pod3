#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "can.h"

command_status do_can(int argc, char *argv[]) {
	printf("%s:  (%d args given)\r\n", argv[0], argc);
	
	//Usage: can read
	if(!strcmp("read", argv[1])){	

		printf("\r\ncalling can_read()\r\n");
		can_read(&hcan);

	//Usage: can send <can ID> <data>
	//
	} else if(!strcmp("send", argv[1])){
		
		
		uint16_t can_id = (uint16_t) atoi(argv[2]);
		size_t length = strlen(argv[3]);
		char *data = strdup(argv[3]);
		printf("can_id: %d, length: %d, data %s", can_id, length, data);
		
		if( length > 0 && length <= 8){
			can_send(can_id, length, (uint8_t*)data, &hcan);
		}
		else{
			printf("Data must be <= 8 chars and > 0.");
			return ERROR;
		}
	}
	

	return SUCCESS;
}

COMMAND_ENTRY(
	"can",
	"can send",
	"can read",
	do_can
)
