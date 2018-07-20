#include "nav_data.h"
#include "solenoid.h"
#include <stdio.h>
#include "retro.h"
#include "gpio.h"
#include "adc.h"

Nav_Data navData = {
     .solenoids = {
         .solenoid_1 = {PRIM_BRAKING_1, NOT_ACTUATED},
         .solenoid_2 = {PRIM_BRAKING_2, NOT_ACTUATED},
         .solenoid_4 = {SEC_VENTING,    NOT_ACTUATED},
         .solenoid_6 = {SEC_BRAKING_1,  NOT_ACTUATED},
         .solenoid_7 = {SEC_BRAKING_2,  NOT_ACTUATED}
     },
     .retros = {0, 0, 0, 0},
     .motion = {0, 0, 0, 0, 0},
     .linePressures = {0, 0, 0, 0, 0, 0}
};

int nav_DAQ(Nav_Data *navDataPtr) {
	/* DEBUG SOLENOIDS */
	int temp = navDataPtr->solenoids.solenoid_1.state;
	int temp2 = navDataPtr->solenoids.solenoid_2.state;
	int temp3 = navDataPtr->solenoids.solenoid_4.state;
	int temp4 = navDataPtr->solenoids.solenoid_6.state;
	int temp5 = navDataPtr->solenoids.solenoid_7.state;
	int vel,acc,pos;

	getTelemetry( &pos, &vel, &acc);

	navDataPtr->motion.accX = acc;
	navDataPtr->motion.velX = vel;
	navDataPtr->motion.posX = pos;

    navDataPtr->limits.limitValue1 = gpio_readPin( GPIOA, 3);
    navDataPtr->limits.limitValue2 = gpio_readPin( GPIOA, 6);
    navDataPtr->limits.limitValue3 = gpio_readPin( GPIOA, 7);

    navDataPtr->volt_adc = adc_read( );

	int successCheck;
	navDataPtr->retros.retroAgreement = getStripCount(&successCheck);
	printf("Strip Count: %u\r\n", navDataPtr->retros.retroAgreement);
	harvest_solenoids(&(navDataPtr->solenoids));

	if (navDataPtr->solenoids.solenoid_1.state != temp) printf("CHANGE1\r\n");
	if (navDataPtr->solenoids.solenoid_2.state != temp2) printf("CHANGE2\r\n");
	if (navDataPtr->solenoids.solenoid_4.state != temp3) printf("CHANGE4\r\n");
	if (navDataPtr->solenoids.solenoid_6.state != temp4) printf("CHANGE6\r\n");
	if (navDataPtr->solenoids.solenoid_7.state != temp5) printf("CHANGE7\r\n");
	
    return 0; // on Success
}
