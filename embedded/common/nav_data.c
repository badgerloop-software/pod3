#include "nav_data.h"
#include "solenoid.h"

int nav_DAQ(Nav_Data *navData) {
	harvest_solenoids(&(navData->solenoids));	
	
	return 0; // on Success
}
