#include <stdio.h>

#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "can.h"


#define BLINK_INTERVAL	250

/* Nucleo 32 I/O */

//Limit Switches
//Lim1: Door engaged
//Lim2: Primary braking engaged
//Lim3: Secondary Braking engaged
FILL_GPIO(LIM1, GPIOA, 3, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(LIM2, GPIOA, 6, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(LIM3,	GPIOA, 7, INPUT, LOW_SPEED, NONE, true, SENSOR);

//Retros
// Retros 1 2 and 3
FILL_GPIO(RETRO1, GPIOA, 0, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(RETRO2, GPIOA, 1, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(RETRO3, GPIOA, 5, INPUT, LOW_SPEED, NONE, true, SENSOR);

//IMU
FILL_GPIO(IMU_nRST,	GPIOB, 1, OUTPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_DRDY,	GPIOC, 14, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_SYNC_OUT,	GPIOC, 15, INPUT, LOW_SPEED, NONE, true, SENSOR);
FILL_GPIO(IMU_SYNC_IN,	GPIOA, 8, OUTPUT, LOW_SPEED, NONE, true, SENSOR);

//SPI
FILL_AFIO(SPI3_SCLK, GPIOB, 3, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_MOSI, GPIOB, 5, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_MISO, GPIOB, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(SPI3_SSEL, GPIOA, 4, ALT, 6, LOW_SPEED, NONE, true, OTHER );

//UART
FILL_AFIO(UART1_TX, GPIOB, 6, ALT, 7, LOW_SPEED, NONE, true, OTHER );
FILL_AFIO(UART1_RX, GPIOB, 7, ALT, 7, LOW_SPEED, NONE, true, OTHER );

//I2C
//Solenoid (PCF8574)
/*
U1 I2C Address 0x100
	P0: SOL1: Primary Braking Trigger
	P1: SOL2: Primary Braking Venting
	P2: SOL3: Secondary Braking Trigger
	P3: SOL4: Secondary Braking Venting
	P4: SOL5: Not planned
	P5: SOL6: Not Planned
	P6: SOL7: Not planned
	P7: SOL8: Not planned
   */

//Pressure
//ADC (PCF8591)
/*
U7 I2C Address 0x48:
	AIN0: DUCER1: Primary Tank
	AIN1: DUCER2: Secondary Tank
	AIN2: DUCER3: Primary Upstream
	AIN3: DUCER4: Primary Downstream
U8 I2C Address: 0x49:
	AIN0: DUCER5: Secondary Upstream
	AIN1: DUCER6: Secondary Downstream
	AIN2: DUCER7: Primary Brakes
	AIN3: DUCER8: Distance Sensor

 	*/

/* CAN Globals */
CAN_HandleTypeDef     hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

inline void printPrompt(void) {
	fputs("[nav-build] $ ", stdout);
	fflush(stdout);
}

int nav_init(void) {

	/* nav specific initializations */

	return 0;
}


int nav_telemetry_send(void){
	uint32_t can_id = 0x106;
	size_t length = 8;
	uint8_t TxData[8];
	
	// First Telemetry Message Setup
	TxData[0] = 0; // Position
	TxData[1] = 0; // Velocity
	TxData[2] = 0; // Acceleration -X
	TxData[3] = 0; // Acceleration -Y
	TxData[4] = 0; // Acceleration -Z
	TxData[5] = 0; // Tape Count 
	TxData[6] = 0; // Tape Count (1 bit)
	TxData[7] = 0; // Not Used
	
	can_send(can_id, length, TxData, &hcan);


	// Second Telemetry Message Setup
	can_id = 0x107;
	TxData[0] = 0; // Pressure 1 (high bytes)
	TxData[1] = 0; // Pressure 1 (low bytes)
	TxData[2] = 0; // Pressure 2 (high bytes)
	TxData[3] = 0; // Pressure 2 (low bytes)
	TxData[4] = 0; // Pressure 3 (high bytes)
	TxData[5] = 0; // Pressure 3 (low bytes)
	TxData[6] = 0; // Pressure 4 (low bytes)
	TxData[7] = 0; // Pressure 4 (low bytes)


	can_send(can_id, length, TxData, &hcan);


	can_id = 0x108;
	TxData[0] = 0; // Pressure 5 (high bytes)
	TxData[1] = 0; // Pressure 5 (low bytes)
	TxData[2] = 0; // Pressure 6 (high bytes)
	TxData[3] = 0; // Pressure 6 (low bytes)
	TxData[4] = 0; // Solenoid 1 driven (bit 0)... Solenoid 6 Driven (bit 5)
	TxData[5] = 0; // Should Stop == True if byte 5 == 1
	TxData[6] = 0; // Unused
	TxData[7] = 0; // Unused

	can_send(can_id, length, TxData, &hcan);

	return 0;
}




void nav_receive_telemetry(uint32_t can_id, uint8_t * RxData){
	//TODO update with incomming CAN messages
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
		// TODO fault handler
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
	if (io_init() || periph_init(&hcan) || nav_init())
		fault();

	rx = get_rx(USB_UART);

	post("Navigation");
	printPrompt();

	while (1) {
		check_input(rx);
		blink_handler(BLINK_INTERVAL);
	
		nav_telemetry_send();
	}

	return 0;
}
