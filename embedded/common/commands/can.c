#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "can.h"
command_status do_can(int argc, char *argv[]) {
	printf("%s:  (%d args given)\r\n", argv[0], argc);
	
		
	if(!strcmp("read", argv[1])){	

		printf("\r\ncalling can_read()\r\n");
		can_read();

	} else if(!strcmp("send", argv[1])){
		uint16_t can_id = (uint16_t) ~((unsigned int) argv[2]);
		uint64_t length = (uint64_t) ~((unsigned int) argv[3]);
		uint64_t data =   (uint64_t) ~((unsigned int) argv[4]);
		printf("\r\ncalling can_send with params \r\n");
		can_send(can_id, length, data);
	}
	

	return SUCCESS;
}

COMMAND_ENTRY(
	"can",
	"can send",
	"can read",
	do_can
)
