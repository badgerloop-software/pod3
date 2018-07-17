#ifndef _PCF8591__H__
#define _PCF8591__H__

#include <stdint.h>
#include <stdbool.h>

#define PCF8591_1_ADDR 0x48
#define PCF8591_2_ADDR 0x49

typedef enum _adci2c_pin_mapping {
	PS1 = 0,
	PS2 = 1,
	PS3 = 2,
	PS4 = 3,
	ADCI2C_INVALID = 4
} adci2c_pin_t;

bool adci2c_start_read(uint8_t addr);
bool adci2c_read(uint8_t addr, uint8_t val);
adci2c_pin_t str_to_pin(const char *str);
void adci2c_dump(void);
extern const char *iox_pin_str[];

#endif

