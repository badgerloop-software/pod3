#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "uart.h"
#include "dashboard_control.h"
#include "can.h"

#define BLINK_INTERVAL	250
uint8_t transition_active;
/* Nucleo 32 I/O */
//THERM1: between batteries 1 and 2
//THERM2: between batteries 2 and 3
FILL_GPIO(THERM1,  GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(THERM2,  GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR)

//SENSOR
//VOLT_SENSE is 12V battery voltage
//CURR_SENSE is 12V battery current draw, charging not included
FILL_GPIO(VOLT_SENSE, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(CURR_SENSE, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR)

//UART
FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER )

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER )

//SD
FILL_GPIO(SD_CS,	 GPIOB, 0, OUTPUT, LOW_SPEED, NONE, true, OTHER )

/* CAN Globals */
CAN_HandleTypeDef     hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

inline void printPrompt(void) {
	fputs("[dash-build] $ ", stdout);
	fflush(stdout);
}

int dash_init(void) {
	/* dash specific initializations */
	return 0;
}







int ccp_telemetry_send(void){
	// Master State Message
	uint32_t can_id = 0x0d0;
	size_t length = 8;
	uint8_t TxData[8];
	TxData[0] = 0;
	TxData[1] = 0;
	TxData[2] = 0;
	TxData[3] = 0;
	TxData[4] = 0;
	TxData[5] = 0;
	TxData[6] = 0;
	TxData[7] = 0;
	
	//TODO Replace with actual get state message
	uint8_t current_state = 0;

	//TODO replace states with ENUM and state parsing
	if(current_state == 0) TxData[0] = 0; //Idle
	else if (current_state == 1) TxData[0] = 1; //Ready_for_pumpdown
	else if (current_state == 2) TxData[0] = 2; //pumpdown
	else if (current_state == 4) TxData[0] = 4; //ready
	else if (current_state == 8) TxData[0] = 8; //propulsion_start_hyperloop
	//TODO complete table or replace with ENUM

	
	can_send(can_id, length, TxData, &hcan);

	// Transition State Message
	can_id = 0x0d1;
	uint8_t transition_command = 0; //temporary
	
	
	
	uint8_t transition_active = 0; //temporary	
	//TODO replace with transition state message
	if(transition_active == 1) TxData[0] = 1;
	if(transition_command == 0) TxData[1] = 0; //transition to Idle
	else if (transition_command == 1) TxData[1] = 1; //transition to Ready_for_pumpdown
	else if (transition_command == 2) TxData[1] = 2; //transition to pumpdown
	else if (transition_command == 4) TxData[1] = 4; //transition to ready
	else if (transition_command == 8) TxData[1] = 8; //transition to propulsion_start_hyperloop
	
	can_send(can_id, length, TxData, &hcan);

	// Overwrite commands
	can_id = 0x0d2;
	uint8_t overwrite_command = 0; //temporary
	if(overwrite_command == 0) TxData[1] = 0; //transition to Idle
	else if (overwrite_command == 1) TxData[0] = 1; //transition to Ready_for_pumpdown
	else if (overwrite_command == 2) TxData[0] = 2; //transition to pumpdown
	else if (overwrite_command == 4) TxData[0] = 4; //transition to ready
	else if (overwrite_command == 8) TxData[0] = 8; //transition to propulsion_start_hyperloop

	can_send(can_id, length, TxData, &hcan);

	// Fault
	can_id = 0x0d3;
	uint8_t ccp_fault_command = 0; //temporary
	if(ccp_fault_command == 0) TxData[1] = 0; //transition to Idle
	else if (ccp_fault_command == 1) TxData[0] = 1; //transition to Ready_for_pumpdown
	else if (ccp_fault_command == 2) TxData[0] = 2; //transition to pumpdown
	else if (ccp_fault_command == 4) TxData[0] = 4; //transition to ready
	else if (ccp_fault_command == 8) TxData[0] = 8; //transition to propulsion_start_hyperloop

	// Telemetry
	// TODO update with sensor readings
	can_id = 0x0d4;
	TxData[0] = 0; //pressure sensor LSB
	TxData[1] = 0; //pressure sensor MSB
	TxData[2] = 0; //lv battery therm 1
	TxData[3] = 0; //lv battery therm 2
	TxData[4] = 0; //lv battery votlage
	TxData[5] = 0; //lv battery current
	TxData[6] = 0; //unused
	TxData[7] = 0; //unused

	can_send(can_id, length, TxData, &hcan);

	return 0;
}

void dash_receive_telemetry(uint32_t can_id, uint8_t * RxData){
	int i;
	if (can_id == 0x0d6){
		printf("received telmetry from %lx\r\n", can_id);
		for(i = 0; i < 8; i++){
			printf("RxData[%d]: %x\r\n", i, RxData[i]);
		}
	}
}

int main(void) {
	PC_Buffer *rx;
	PC_Buffer *ctrl_rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(&hcan) || dash_init())
		fault();
	rx = get_rx(USB_UART);
	ctrl_rx = get_rx(USART1);

	post("Dashboard");
	printPrompt();
	while (1) {
		check_input(rx);
		check_incoming_controls(ctrl_rx);
		blink_handler(BLINK_INTERVAL);


		/* Check for incoming CAN messages */
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)){
			HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
			if(RxHeader.StdId == 0x0d6){
				dash_receive_telemetry(RxHeader.StdId, RxData);
			}
		}
	}
	return 0;
}
