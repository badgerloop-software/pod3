#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "can.h"

#define BLINK_INTERVAL	250

/* Nucleo 32 I/O */
//Shutdown Circuit
FILL_GPIO(MSTR_SW_FDBK, GPIOB, 0, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(E_STOP_FDBK, 	GPIOB, 7, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(PV_LIM_FDBK, 	GPIOB, 6, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(HVD_FDBK, 	GPIOB, 1, INPUT, LOW_SPEED, NONE, true, SENSOR)
FILL_GPIO(BMS_STAT_FDBK, GPIOC, 14, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(IMD_STAT_FDBK, GPIOC, 15, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(HV_EN_FDBK, 	GPIOA, 8, INPUT, LOW_SPEED, NONE, true, OTHER)
//HVD_MCU is a spare limit switch input
FILL_GPIO(HVD_MCU,  GPIOA, 4, INPUT, LOW_SPEED, NONE, true, OTHER)

//SOLENOIDS
//FILL_GPIO(SOL1, 	GPIOA, 5, OUTPUT, LOW_SPEED, NONE, true, VALVE)
//FILL_GPIO(SOL2, 	GPIOA, 1, OUTPUT, LOW_SPEED, NONE, true, VALVE)

//Voltage Divider
//FILL_GPIO(VD2, 	GPIOA, 3, INPUT, LOW_SPEED, NONE, true, OTHER)

//MCU
//FILL_GPIO(LIM_MCU, 		GPIOA, 2, INPUT, LOW_SPEED, NONE, true, OTHER)
FILL_GPIO(MCU_HV_EN, 		GPIOA, 6, OUTPUT, LOW_SPEED, NONE, true, OTHER)

//MISC
//High Voltage Disconnect Status	
FILL_GPIO(HVD_STATUS, 	GPIOA, 7, INPUT, LOW_SPEED, NONE, true, OTHER)
//Motor GPIO circuit output
FILL_GPIO(DIN8, GPIOB, 5, OUTPUT, LOW_SPEED, NONE, true, OTHER)


/* CAN Globals */
CAN_HandleTypeDef     hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;



inline void printPrompt(void) {
	fputs("[pv-build] $ ", stdout);
	fflush(stdout);
}

int pv_init(void) {

	/* nav specific initializations */

	return 0;
}


int pv_telemetry_send(void){
	uint32_t can_id = 0x0D6;
	size_t length = 8;
	uint8_t TxData[8];
	
	// First Telemetry Message Setup
	
	//Byte 0:
	//Bit 0: HV_EN_FDBK Enabled
	//Bit 1: Master Switch Enabled
	//Bit 2: Estop Disabled
	//Bit 3: Limit Switch Closed
	//Bit 4: Manual Service Disconnect Enabled
	//Bit 5: BMS_Enable
	//Bit 6: IMD_Enabled
	//Bit 7: MCU_Enabled

	uint8_t shutdown_circuit_status = 0;
	if (gpio_readPin(GPIOA, 8))	 shutdown_circuit_status += 1;
	//Master Switch feedback
	if (gpio_readPin(GPIOB, 0))	 shutdown_circuit_status += 2;
	//EStop feeback
	if (gpio_readPin(GPIOB, 7))	 shutdown_circuit_status += 4;
	//Limit Switch feedback
	if (gpio_readPin(GPIOB, 5)) 	 shutdown_circuit_status += 8;
	//Manual Service Disconnect feedback
	if (gpio_readPin(GPIOB, 1))	 shutdown_circuit_status += 16;
	//BMS Enabled
	if (gpio_readPin(GPIOC, 14))	 shutdown_circuit_status += 32;
	//IMD Enabled
	if (gpio_readPin(GPIOC, 15))	 shutdown_circuit_status += 64;
	//MCU_Enabled
	if (gpio_readPin(GPIOA, 6)) 	 shutdown_circuit_status += 128;

	printf("Shutdown_Circuit_status %d\r\n", shutdown_circuit_status);

	TxData[0] = shutdown_circuit_status; // Position
	TxData[1] = 0; // Unused 
	TxData[2] = 0; // Unused 
	TxData[3] = 0; // Unused
	TxData[4] = 0; // Unused
	TxData[5] = 0; // Unused 
	TxData[6] = 0; // Unused
	TxData[7] = 0; // Unused
	
	can_send(can_id, length, TxData, &hcan);

	//Fault and warnings 
	can_id = 0x0D8;
	
	uint8_t fault_status = 0;
	//if(pressure vessel pressure too low) fault_status = 1;
	//if(isolation fault) fault_status += 2;
	//if(motor fault) fault_status += 4;
	//if(bms fault) fault_status += 8;
	
	uint8_t warning_status = 0;
	//if (battery temp out of range) warning_status = 1;
	//if (motor temp out of range) warning_status += 2;
	//if (motor rpm out of range) warning_status += 4;
	//if (battery voltage out of range) warning_status += 8;
	//if (battery current out of range) warning_status += 16;
	
	TxData[0] = fault_status; // Fault 
	TxData[1] = warning_status; // Warning
	TxData[2] = 0; // Unused 
	TxData[3] = 0; // Unused
	TxData[4] = 0; // Unused
	TxData[5] = 0; // Unused 
	TxData[6] = 0; // Unused
	TxData[7] = 0; // Unused

	return 0;
}



void pv_receive_telemetry(uint32_t can_id, uint8_t * RxData){
	printf("received telmetry from %lx\r\n", can_id);
	for(int i = 0; i < 8; i++){
		printf("RxData[%d]: %x\r\n", i, RxData[i]);
	}
	if(can_id == 0x0d0){
		//TODO update with State handler update
	} else if (can_id == 0x0d1){
		//TODO update with State transition start
	} else if (can_id == 0x0d2){
		//TODO actuation overwrite
	} else if (can_id == 0x0d3){
		// TODO ccp fault handler
	} else if (can_id == 0x0d4){
		// TODO update ccp telemetry vals
	} else if (can_id == 0x0d6){
		// TODO update PV telemetry vals
	} else if (can_id == 0x0d8){
		// TODO handle PV faults
	} 
}


int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(&hcan) || pv_init())
		fault();

	rx = get_rx(USB_UART);

	post("Pressure Vessel");
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
		pv_telemetry_send();


		/* Check for incoming CAN messages */
		if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)){
			HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
			if(RxHeader.StdId == 0x0d6){
				pv_receive_telemetry(RxHeader.StdId, RxData);
			}
		}
	}
	

	return 0;
}
