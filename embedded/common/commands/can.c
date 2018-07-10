#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "commands.h"
#include "board.h"
#include "can.h"

command_status try_send(uint32_t can_id, uint32_t mailbox, size_t length, uint8_t *TxData) {
	if (can_send(can_id, mailbox, length, TxData) != HAL_OK)
		return FAIL;
	return CMD_SUCCESS;	
}

command_status can_try_read(void) {

	uint8_t i;
	HAL_StatusTypeDef retval = FAIL;

	if (HAL_CAN_GetRxFifoFillLevel(&can_handle, CAN_RX_FIFO0)) {
		retval = HAL_CAN_GetRxMessage(& can_handle, CAN_RX_FIFO0, &RxHeader, RxData);
		           
		/* Printing out received data */
		printf("Received CAN ID: #%lx\r\n", RxHeader.StdId );
		for (i = 0; i < 8; i++) {
			if (RxData[i] != 0) {
				printf("CAN Message Data #%d: %x\r\n", i, RxData[i]);
			}
		}
	}

	return (retval == HAL_OK) ? CMD_SUCCESS : FAIL;	
}

command_status do_can(int argc, char *argv[]) {
	
	if (argc == 1) return USAGE;

	/* read a can message */
	if (!strcmp("read", argv[1]))
		return can_try_read();
	
	/* send a can test message */
	if (!strcmp("send_test", argv[1])){
		//TEST MESSAGE SEND
		uint32_t can_id = 0x123;
		uint8_t length = 8;
		uint64_t data = 0x5555555555555555;		
		uint8_t TxData[8];
		memset(TxData, data, 16);

		// TODO, use 'data' as-is
		return try_send(can_id, 0, length, TxData);
	}

	if (!strcmp("send_inter", argv[1])){
		
		uint8_t TxData[8];
		
		printf("DASH: %d NAV_REC: %d\r\n", DASH, NAV_REC);
		can_send_intermodule(DASH, NAV_REC, 1, TxData);
	
	}

	if (argc == 2 || argc == 3 || argc == 4) return USAGE;

	if (!strcmp("send", argv[1])) {
		
		char * str;
		uint32_t can_id = strtol(argv[2], &str, 16);
		if (can_id > 0x7ff) return ERROR;
		
		size_t length = strlen(argv[3]);

		uint8_t data[8];
		long long converted = strtoll(argv[3], &str, 16);

		if (converted == LONG_MAX || converted == LONG_MIN) {
			printf("strtoll() error.\r\n" );
		}

		else if (str == argv[3]) {
			printf("No digits were found.\r\n");
		}

		else if (*str != '\0') {
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

		can_send(can_id, 0, length, data);
	}

	return USAGE;
}

COMMAND_ENTRY(
	"can",
	"can <subcommand> [args...]\r\n\r\n"
	"read\r\n"
	"send_test\r\n"
	"send <TODO>\r\n",
	"Interact with the CAN subsystem.",
	do_can
)
