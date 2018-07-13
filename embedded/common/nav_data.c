#include "nav_data.h"
#include "solenoid.h"

int nav_DAQ(Solenoid_States *solenoids) {
	harvest_solenoids(solenoids);	
	
	return 0; // on Success
}
