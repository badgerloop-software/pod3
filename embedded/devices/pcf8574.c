#include <stdio.h>
#include <string.h>
#include "iox.h"
#include "i2c.h"

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

uint8_t _iox_commanded_val = 0x0;
uint8_t _iox_read_val;
bool iox_read_stale = true;

bool iox_start_read(void) {
	return (i2c_start_read(PCF8574_ADDR, 1) == HAL_OK) ? true : false;
}

bool iox_read(uint8_t *val) {
	if (!i2c_read_ready()) return false;
	_iox_read_val = i2c_rx[0];
	i2c_clear_flag(I2C_RX_READY);
	*val = _iox_read_val;
	iox_read_stale = false;
	return true;
}

bool iox_set(iox_pin_t pin) {
	_iox_commanded_val |= 1 << pin;
	if (i2c_start_write(PCF8574_ADDR, 1, &_iox_commanded_val) == HAL_OK)  {
		iox_read_stale = true;
		return true;
	}
	return false;
}

bool iox_clear(iox_pin_t pin) {
	_iox_commanded_val &= ~(1 << pin);
	if (i2c_start_write(PCF8574_ADDR, 1, &_iox_commanded_val) == HAL_OK)  {
		iox_read_stale = true;
		return true;
	}
	return false;
}

const char *pin_to_str(iox_pin_t pin) {
	return iox_pin_str[pin];
}

iox_pin_t str_to_pin(const char *str) {
	int i;
	for (i = 0; i < 8; i++) {
		if (!strcmp(iox_pin_str[i], str))
			return ((iox_pin_t) i);
	}
	return IOX_INVALID;
}

bool iox_get_staleness(void) {
	return iox_read_stale;
}

void iox_dump(void) {
	uint8_t i;
	printf("%s:\r\n", iox_read_stale ? "**STALE**" : "VALID");
	for (i = 0; i < 8; i++) {
		printf("%s: %s\r\n", iox_pin_str[i], (_iox_read_val & (0x1 << i)) ? "HIGH" : "LOW");
	}
}
