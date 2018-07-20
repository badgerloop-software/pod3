#include <stdio.h>
#include <time.h>
#include "iox.h"
#include "dashboard_data.h"
#include "system.h"
#include "board.h"
#include "console.h"
#include "usart.h"
#include "pin_alias.h"
#include "solenoid.h"
#include "can.h"
#include "nav_data.h"
#include "exti.h"
#include "state_machine.h"
#define BLINK_INTERVAL	250
#define DAQ_INTERVAL    100
#define STATE_INTERVAL  100 
#define TELEM_INTERVAL  100
#define HRTBT_INTERVAL  100

const int board_type = NAV;
extern volatile unsigned int ticks;
extern Nav_Data navData;
state_box nav_stateVal = {3, 0};
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

inline void printPrompt(void) {
	fputs("[nav-build] $ ", stdout);
	fflush(stdout);
}

int nav_init(void) {

	/* nav specific initializations */

    GPIO_TypeDef *gpioa = GPIOA;
    
	change_solenoid(PRIM_BRAKING_1, ACTUATED);
	change_solenoid(PRIM_BRAKING_2, NOT_ACTUATED);
	change_solenoid(SEC_VENTING, ACTUATED);
	change_solenoid(SEC_BRAKING_1, NOT_ACTUATED);
	change_solenoid(SEC_BRAKING_2, NOT_ACTUATED);

    /* Retro 1 is on pin PA0
     * Retro 2 is on pin PA1
     * Retro 3 is on pin PA5 */

    /* EXTI Init */ 
    //Each Pin falling-edge interrupt enabled

    //Pin 0 EXTI Config (RETRO1)
    exti_config(gpioa, 0, 0, 1, 1);
    //Pin 1 EXTI Config (RETRO2)
    exti_config(gpioa, 1, 0, 1, 1);
    //Pin 5 EXTI Config (RETRO3)
    exti_config(gpioa, 5, 0, 1, 1);
     
    //Pin 3 EXTI Config (LIM1)
    exti_config(gpioa, 3, 0, 1, 1);
    //Pin 6 EXTI Config (LIM2)
    exti_config(gpioa, 6, 0, 1, 1);
    //Pin 7 EXTI Config (LIM3)
    exti_config(gpioa, 7, 0, 1, 1);
    
    return 0;
}

extern state_t state_handle;
int main(void) {

	PC_Buffer *rx;

	/* initialize pins and internal interfaces */
	if (io_init() || periph_init(NAV) || nav_init())
		fault();
	
	rx = get_rx(USB_UART);
	
	post("Navigation");
	printPrompt();

	
	unsigned int lastDAQ = 0, lastState = 0, lastTelem = 0, lastHrtbt = 0;
	unsigned int currDAQ = 0, currState = 0, currTelem = 0, currHrtbt = 0;
	while (1) {
		currDAQ = (ticks + 15) / 100;
		currState = (ticks + 30) / 100;
		currTelem = (ticks + 45) / 100;
		currHrtbt = (ticks + 60) / 100;
		
		if (can_read() == HAL_OK){
		   	if (board_can_message_parse(BADGER_CAN_ID, RxData) == HAL_ERROR) {
				printf("BIG ERROR");
			}; 
		}
		if(currDAQ != lastDAQ){
			if (nav_DAQ(&navData)) printf("DAQ Failure");
			lastDAQ = currDAQ;
		}
		if(currState != lastState){
			iox_start_read();
			//printf("NAV STATE: %u\r\n", state_handle.curr_state); 
			state_machine_handler();
			lastState = currState;
		}
		if(currTelem != lastTelem){
			board_telemetry_send(board_type);
			lastTelem = currTelem;
		}
		if(currHrtbt != lastHrtbt){
			lastHrtbt = currHrtbt;
		}

		check_input(rx);
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
