#include <string.h>
#include "pin_alias.h"

const char *GROUP_NAMES[] = {
	"OTHER", "SENSOR", "VALVE", "RETRO", "LED", "I2C", "PRESSURE", "CAN_GROUP"
};
const uint8_t NUM_GROUPS = sizeof(GROUP_NAMES) / sizeof(char *);

const GPIO *GPIO_TABLE = (GPIO *) &__GPIO_ENTRIES_START;

int gpioAliasInit(void) {
	unsigned int i;
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
	unsigned int i;
	for (i = 0; i < NUM_GPIO_ALIAS ;i++) {
		if (!strcmp(GPIO_TABLE[i].name, name)) {
			*pin = GPIO_TABLE[i].pin;
			*port = GPIO_TABLE[i].port;
			return 1;
		}
	}
	return 0;
}

int getGpioAlias(GPIO_TypeDef **port, uint8_t *pin, GPIO *alias) {
	unsigned int i;
	for (i = 0; i < NUM_GPIO_ALIAS; i++) {
		if (GPIO_TABLE[i].pin == *pin && GPIO_TABLE[i].port == *port) {
			*alias = GPIO_TABLE[i];
			return 1;
		}	
	}
	return 0;
}

int getGpioAliasIndex(const char *str) {
	unsigned int i;
	for (i = 0; i < NUM_GROUPS; i++) {
		if (!strcmp(str, GROUP_NAMES[i]))
			return i;
	}
	return -1;
}
