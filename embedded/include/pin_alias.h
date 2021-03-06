#ifndef PIN_ALIAS__H
#define PIN_ALIAS__H

#include "gpio.h"

#define FILL_AFIO(_name, _port, _pin, _mode, _af_val, _speed, _pullup, _usable, _group) \
	 const GPIO __attribute__((section (".GPIO"))) _name##_pin = {\
		.name = #_name,\
		.port = _port,\
		.pin = _pin,\
		.mode = _mode,\
		.speed = _speed,\
		.pullup = _pullup,\
		.usable = _usable,\
		.af_val = _af_val,\
		.group = _group\
	};

#define FILL_GPIO(_name, _port, _pin, _mode, _speed, _pullup, _usable, _group) \
	FILL_AFIO(_name, _port, _pin, _mode, 0, _speed, _pullup, _usable, _group)

extern unsigned int __GPIO_ENTRIES_END, __GPIO_ENTRIES_START;
#define NUM_GPIO_ALIAS (\
	(((unsigned int) &__GPIO_ENTRIES_END - (unsigned int) &__GPIO_ENTRIES_START)/sizeof(GPIO))\
)

extern const char * GROUP_NAMES[]; 
extern const uint8_t NUM_GROUPS; 
extern const GPIO *GPIO_TABLE;

int gpioAliasInit(void);
int hasGpioAlias(GPIO_TypeDef ** port, uint8_t * pin, char * name);
int getGpioAlias(GPIO_TypeDef ** port, uint8_t * pin, GPIO * alias);
int getGpioAliasIndex(const char *str);

#endif
