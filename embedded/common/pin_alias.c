#include <string.h>
#include "pin_alias.h"

const char * GROUP_NAMES[] = {
	"OTHER", "SENSOR", "VALVE", "RETRO", "LED", "I2C", "PRESSURE"
};
const uint8_t NUM_GROUPS = sizeof(GROUP_NAMES) / sizeof(char *);

const GPIO GPIO_TABLE[] = {

	/* STM32L432KC */
	// TODO

	/* Nucleo 32 I/O */
	FILL_GPIO("LED",		GPIOB, 3, OUTPUT, LOW_SPEED, NONE, true, LED)
	FILL_AFIO("UART TX",	GPIOA, 2, ALT, 7, MEDIUM_SPEED, NONE, true, OTHER)
	FILL_AFIO("UART RX",	GPIOA, 15, ALT, 3, MEDIUM_SPEED, NONE, true, OTHER)

	/* CAN */
	// TODO

	/* I2C */
	// TODO

};
const uint8_t NUM_GPIO_ALIAS = sizeof(GPIO_TABLE) / sizeof(GPIO);

int gpioAliasInit(void) {
	int i;
	GPIO_TypeDef * port;
	uint8_t pin;

	for(i = 0; i < NUM_GPIO_ALIAS; i++){
		if (GPIO_TABLE[i].usable){
			port = GPIO_TABLE[i].port;
			pin = GPIO_TABLE[i].pin;

			gpio_setClock(port, true);
			gpio_setMode(port, pin, GPIO_TABLE[i].mode);
			gpio_setSpeed(port, pin, GPIO_TABLE[i].speed);

			if (GPIO_TABLE[i].mode == ALT)
				gpio_setAlternateFunc(port, pin, GPIO_TABLE[i].af_val);

			if (GPIO_TABLE[i].group == I2C)
				gpio_openDrainState(port, pin, true);

			//if (GPIO_TABLE[i].group == RETRO)
			//	exti_config(port, pin, true, false, true);

			if (GPIO_TABLE[i].mode == OUTPUT)
				gpio_writePin(port, pin, 0);
		}
	}
	return 0;
}

int hasGpioAlias(GPIO_TypeDef **port, uint8_t *pin, char *name) {
	int i;
	for (i = 0; i< NUM_GPIO_ALIAS ;i++) {
		if (!strcmp(GPIO_TABLE[i].name, name)) {
			*pin = GPIO_TABLE[i].pin;
			*port = GPIO_TABLE[i].port;
			return 1;
		}
	}
	return 0;
}

int getGpioAlias(GPIO_TypeDef **port, uint8_t *pin, GPIO *alias) {
	int i; 
	for (i = 0; i < NUM_GPIO_ALIAS; i++) {
		if (GPIO_TABLE[i].pin == *pin && GPIO_TABLE[i].port == *port) {
			*alias = GPIO_TABLE[i];
			return 1;
		}	
	}
	return 0;
}
