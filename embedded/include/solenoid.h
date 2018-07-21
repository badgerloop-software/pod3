#ifndef SOLENOID__H
#define SOLENOID__H

#include <stdbool.h>

#define ACTUATED 	1
#define NOT_ACTUATED 	0

#define P1 0
#define P2 4
#define S1 7
#define S2 6
#define S3 3

typedef struct {
	int  name;
	int  state;
} Solenoid_t;

typedef struct {
	Solenoid_t solenoid_1;
	Solenoid_t solenoid_2;
	Solenoid_t solenoid_4;
	Solenoid_t solenoid_6;
	Solenoid_t solenoid_7;
} Solenoid_States;
bool actuate_brakes();
bool init_solenoids();
bool actuate_sec_brakes();
bool vent_brakes();
void harvest_solenoids(Solenoid_States*);
int change_solenoid(int, int);
#endif
