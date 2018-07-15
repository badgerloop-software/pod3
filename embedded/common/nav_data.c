#include "nav_data.h"
#include "solenoid.h"
#include <stdio.h>

Nav_Data navData = {
     .solenoids = {
         .solenoid_1 = {"prim_braking_1", NOT_ACTUATED},
         .solenoid_2 = {"prim_braking_2", NOT_ACTUATED},
         .solenoid_4 = {"sec_venting",    NOT_ACTUATED},
         .solenoid_6 = {"sec_braking_1",  NOT_ACTUATED},
         .solenoid_7 = {"sec_braking_2",  NOT_ACTUATED}
     },
     .retros = {0, 0, 0, 0},
     .motion = {0, 0, 0, 0, 0},
     .linePressures = {0, 0, 0, 0, 0, 0, 0, 0}
};

int nav_DAQ(Nav_Data *navDataPtr) {
	int temp = navDataPtr->solenoids.solenoid_1.state;
	int temp2 = navDataPtr->solenoids.solenoid_2.state;
	int temp3 = navDataPtr->solenoids.solenoid_4.state;
	int temp4 = navDataPtr->solenoids.solenoid_6.state;
	int temp5 = navDataPtr->solenoids.solenoid_7.state;

	harvest_solenoids(&(navDataPtr->solenoids));	
	if (navDataPtr->solenoids.solenoid_1.state != temp) printf("CHANGE1\r\n");
	if (navDataPtr->solenoids.solenoid_2.state != temp2) printf("CHANGE2\r\n"); 
	if (navDataPtr->solenoids.solenoid_4.state != temp3) printf("CHANGE4\r\n");
	if (navDataPtr->solenoids.solenoid_6.state != temp4) printf("CHANGE6\r\n");
	if (navDataPtr->solenoids.solenoid_7.state != temp5) printf("CHANGE7\r\n");   	
	return 0; // on Success
}
