#include "iox.h"

uint8_t _iox_commanded_val = 0x0;
uint8_t _iox_read_val;

bool iox_start_read(void) {
	return (i2c_start_read(PCF8574_ADDR, 1) == HAL_OK) ? true : false;
}

bool iox_read(uint8_t *val) {
	if (!i2c_read_ready()) return false;
	_iox_read_val = i2c_rx[0];
	*val = _iox_read_val;
	return true;
}

bool iox_set(iox_pin_t pin) {
	_iox_commanded_val |= 1 << pin;
	return (i2c_start_write(
		PCF8574_ADDR, 1, &_iox_commanded_val
	) == HAL_OK) ? true : false;
}

bool iox_clear(iox_pin_t pin) {
	_iox_commanded_val &= ~(1 << pin);
	return (HAL_StatusTypeDef i2c_start_write(
		PCF8574_ADDR, 1, &_iox_commanded_val
	) == HAL_OK_ ? true : false;
}

const char *pin_to_str(iox_pin_t pin) {
	return iox_pin_str[pin];
}
