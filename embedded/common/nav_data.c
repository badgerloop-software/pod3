#include "nav_data.h"
#include "solenoid.h"
#include <stdio.h>
int nav_DAQ(Nav_Data *navData) {
	int temp = navData->solenoids.solenoid_1.state;
	int temp2 = navData->solenoids.solenoid_2.state;
	int temp3 = navData->solenoids.solenoid_4.state;
	int temp4 = navData->solenoids.solenoid_6.state;
	int temp5 = navData->solenoids.solenoid_7.state;

	harvest_solenoids(&(navData->solenoids));	
	if (navData->solenoids.solenoid_1.state != temp) printf("CHANGE1\r\n");
	if (navData->solenoids.solenoid_2.state != temp2) printf("CHANGE2\r\n"); 
	if (navData->solenoids.solenoid_4.state != temp3) printf("CHANGE4\r\n");
	if (navData->solenoids.solenoid_6.state != temp4) printf("CHANGE6\r\n");
	if (navData->solenoids.solenoid_7.state != temp5) printf("CHANGE7\r\n");   	
	return 0; // on Success
}
