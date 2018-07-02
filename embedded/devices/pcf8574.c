#ifndef _IOX__H__
#define _IOX__H__

#include <stdint.h>
#include <stdbool.h>

#define PCF8574_ADDR	0x20

typedef enum _iox_pin_mapping {
	SOL1 = 0,
	SOL2 = 1,
	SOL3 = 2,
	SOL4 = 3,
	SOL5 = 4,
	SOL6 = 5,
	SOL7 = 6,
	SOL8 = 7
} iox_pin_t;

const char *iox_pin_str[] = {
	"SOL1",
	"SOL2",
	"SOL3",
	"SOL4",
	"SOL5",
	"SOL6",
	"SOL7",
	"SOL8"
};

bool iox_start_read(void);

bool iox_read(uint8_t *val);

bool iox_set(iox_pin_t pin);

bool iox_clear(iox_pin_t pin);

#endif
