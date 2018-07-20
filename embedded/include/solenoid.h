#ifndef SOLENOID__H
#define SOLENOID__H

#define ACTUATED 	1
#define NOT_ACTUATED 	0

#define PRIM_BRAKING_1 0
#define PRIM_BRAKING_2 4
#define SEC_VENTING    3
#define SEC_BRAKING_1  6
#define SEC_BRAKING_2  7

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
void actuate_brakes();
void harvest_solenoids(Solenoid_States*);
int change_solenoid(int, int);
#endif
