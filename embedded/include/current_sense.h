#include "adc.h"
#include <stdio.h>
#include "adc.h"
#include <stdlib.h>

#define CURRENT_SENSE_PIN 11

int current_sense_init(void);
double current_sense_read(void);
void state_of_charge(void);
void set_charge_percentage(uint16_t soc);
uint16_t get_time_remaining(void);
