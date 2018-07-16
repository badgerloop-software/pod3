#include <stdio.h>

#include "gpio.h"
#include "shutdown_circuit.h"

void mcu_shutdown_circuit(int state){
	gpio_writePin(GPIOA, 6, state);
}

void rms_power(int state){
	gpio_writePin(GPIOA, 5, state);
}

void read_shutdown_circuit_status(void){
	uint8_t temp = -1;
	temp = gpio_readPin(GPIOB, 0);
	printf("TEMP 1 %d", temp);
	if(temp) SET_MSTR_SW_FDBK;
	else CLR_MSTR_SW_FDBK;

	temp = gpio_readPin(GPIOB, 7);
	printf("TEMP 2 %d", temp);
	if(temp) SET_E_STOP_FDBK;
	else CLR_E_STOP_FDBK;

	temp = gpio_readPin(GPIOB, 6);
	printf("TEMP 3 %d", temp);
	if(temp) SET_PV_LIM_FDBK;
	else CLR_PV_LIM_FDBK;

	temp = gpio_readPin(GPIOB, 1);
	printf("TEMP 4 %d", temp);
	if(temp) SET_HVD_FDBK;
	else CLR_HVD_FDBK;

	temp = gpio_readPin(GPIOC, 14);
	printf("TEMP 5 %d", temp);
	if(temp) SET_BMS_STAT_FDBK;
	else CLR_BMS_STAT_FDBK;

	temp = gpio_readPin(GPIOC, 8);
	printf("TEMP 6 %d", temp);
	if(temp) SET_IMD_STAT_FDBK;
	else CLR_IMD_STAT_FDBK;
	
	temp = gpio_readPin(GPIOA, 8);
	printf("TEMP 7 %d", temp);
	if(temp) SET_HV_EN_FDBK;
	else CLR_HV_EN_FDBK;
}
