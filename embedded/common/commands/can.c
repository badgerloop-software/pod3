#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "commands.h"
#include "board.h"
#include "can.h"

extern volatile uint8_t hb_torque;
extern volatile heartbeat_msg_t hb_status;

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
		uint64_t data = 0x55555555;		
		uint8_t TxData[8];
		memset(TxData, data, 16);

		// TODO, use 'data' as-is
		return try_send(can_id, 0, length, TxData);
	}

	if (!strcmp("send_inter_test", argv[1])){
		
		uint8_t TxData[8];
		
		printf("DASH: %d NAV_REC: %d\r\n", DASH, NAV_REC);
		return can_send_intermodule(DASH, NAV_REC, CAN_TEST_MESSAGE, TxData);
			
	}

	if (!strcmp("listen", argv[1])){
        
        	return can_listen();
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
		
		int i;
		printf("Send data");
		for ( i = 2; i < 8; i++){
			printf(" data[%d]: %x ", i, data[i]);
		}

		can_send(can_id, 0, length, data);
	}
    if(!strcmp("hb_torque", argv[1])){
        hb_torque = atoi(argv[2]);
        /* TESTING */
        printf( "Torque= %d nm\r\n", hb_torque);
    
    } 
    if(!strcmp("hb_next", argv[1])){
        switch( hb_status){
            case IDLE: 
                printf( "Heartbeat Status: Clear Faults\r\n");
                hb_status = FAULTS_CLEARED;
                break;
            case FAULTS_CLEARED:
                printf( "Heartbeat Status: Pre-Run\r\n");
                hb_status = PRE_RUN;
                break;
            case PRE_RUN:
                printf( "Heartbeat Status: Forward\r\n");
                hb_status = PRE_RUN;
                break;
            case FORWARD:
                printf( "Heartbeat Status: Spin Down\r\n");
                hb_status = SPIN_DOWN;
                break;
            case SPIN_DOWN:
                printf( "Heartbeat Status: Discharge\r\n");
                hb_status = DISCHARGE; 
                break;
            case DISCHARGE:
                printf( "Heartbeat Status: Idle\r\n");
                hb_status = POST_RUN;
                break;
            case POST_RUN:
                printf("Heartbeat Status: Post Run\r\n");
                break;
            default: 
                printf("Unknown Heartbeat state.\r\n" );
                break;
        }
    }

	if (argc == 5) return USAGE; 

	if(!strcmp("send_inter", argv[1])){
		BOARD_ROLE sending_module = 0;
  	        RECEIVING_BOARD receiving_board = 0;
	        CAN_MESSAGE_TYPE message_type = 0;
	        if (!strcmp("DEV", argv[2]))
	               sending_module = DEV;
        	else if (!strcmp("DASH", argv[2]))
        	       sending_module = DASH;
	  	else if (!strcmp("NAV", argv[2]))
                	sending_module = NAV;
	        else if (!strcmp("PV", argv[2]))
       		        sending_module = PV;
	        else printf("Select a valid sending board\r\n");

        	if (!strcmp("DEV_REC", argv[3]))
	            	receiving_board = DEV_REC;
       		else if (!strcmp("DASH_REC", argv[3]))
	            	receiving_board = DASH_REC;
        	else if (!strcmp("NAV_REC", argv[3]))
            		receiving_board = NAV_REC;
	        else if (!strcmp("PV_REC", argv[3]))
        	        receiving_board = PV_REC;
	        else if (!strcmp("CCP_NAV_REC", argv[3]))
        		receiving_board = CCP_NAV_REC;
       		else if (!strcmp("CCP_PV_REC", argv[3]))
           		receiving_board = CCP_PV_REC;
	        else if (!strcmp("NAV_PV_REC", argv[3]))
           		receiving_board = NAV_PV_REC;
	        else if (!strcmp("ALL", argv[3]))
        		receiving_board = ALL;
	        else printf("Select a valid receiving board(s)\r\n");

        	if(!strcmp("CAN_TEST_MESSAGE", argv[4]))
		        message_type = CAN_TEST_MESSAGE;
	        else printf("Select a valid message type\r\n");

        	char * str;
			   		    
        	uint8_t data[8];
	        long long converted = strtoll(argv[5], &str, 16);

        	if (converted == LONG_MAX || converted == LONG_MIN) {
		        printf("strtoll() error.\r\n" );
    		}

	        else if (str == argv[5]) {
		    	printf("No digits were found.\r\n");
	      	}	

        	else if (*str != '\0') {
	        	printf("Extra string passed in.\r\n");
	       	}
		
		printf("data input string:	%llx\r\n", converted);
		data[7] = (converted & 0xff0000000000) >> 40;
		data[6] = (converted & 0x00ff00000000) >> 32;
	    	data[5] = (converted & 0x0000ff000000) >> 24;
    		data[4] = (converted & 0x000000ff0000) >> 16;
    		data[3] = (converted & 0x00000000ff00) >> 8;
    		data[2] = (converted & 0x0000000000ff);
        	
		int i;
		for ( i = 2; i < 8; i++){
			printf("Data[%d]:		%x\r\n", i, data[i]);
		}

		return can_send_intermodule(sending_module, receiving_board, message_type, data);
	}    
	return USAGE;
}

COMMAND_ENTRY(
	"can",
	"can <subcommand> [args...]\r\n\r\n"
	"read\r\n"
	"send_test\r\n"
	"send\r\n"
	"send_inter_test\r\n"
	"send_inter <sending_module> <receiving_module> <message_type> <data>\r\n"
	"listen\r\n",
	"Interact with the CAN subsystem.",
	do_can
)
