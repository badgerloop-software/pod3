#ifndef SOLENOID__H
#define SOLENOID__H

#define ACTUATED 	1
#define NOT_ACTUATED 	0

typedef struct {
	char *name;
	int  state;
} Solenoid_t;

typedef struct {
	Solenoid_t solenoid_1;
	Solenoid_t solenoid_2;
	Solenoid_t solenoid_4;
	Solenoid_t solenoid_6;
	Solenoid_t solenoid_7;
} Solenoid_States;

void harvest_solenoids(Solenoid_States*);

#endif
