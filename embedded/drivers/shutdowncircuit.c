#include <string.h>

#include "shutdowncircuit.h"

void mcu_toggle_shutdowncircuit(int state){
	gpio_writePin(GPIOA, 6, state);
	//TODO Add feedback check? 
}

void motor_controller_toggle_power(int state){
	gpio_writePin(GPIOA, 5, state);
	//TODO Add feedback check?
}

void 
