#include "stdio.h"
#include "retro.h"
#include "badgerloop.h"

#define CM_PER_STRIP 3048

//uint8_t master_retro_count = 0;
uint8_t freshness = NOT_FRESH;

void initRetro(void) {
	mainRetro = &RETRO1;
	tape_vel = 0;
}

// for testing purposes only. Fakes a retro count
void incVel(int retro) {
	switch (retro) {
		case 1:
//			printf("Incrementing retro 1\r\n");
			RETRO1.count++;
			RETRO1.prev = RETRO1.curr;
			RETRO1.curr = 1000*RETRO1.count;
			RETRO1.filter[MAINFILTERINDEX] = RETRO1.curr-RETRO1.prev;
			break;
		case 2:
//			printf("Incrementing retro 2\r\n");
			RETRO2.count++;
			RETRO2.prev = RETRO2.curr;
			RETRO2.curr = 1000*RETRO2.count;
			RETRO2.filter[MAINFILTERINDEX] = RETRO2.curr-RETRO2.prev;		
			break;
		case 3:
//			printf("Incrementing retro 3\r\n");
			RETRO3.count++;
			RETRO3.prev = RETRO3.curr;
			RETRO3.curr = 1000*RETRO3.count;
			RETRO3.filter[MAINFILTERINDEX] = RETRO3.curr-RETRO3.prev;
			break;
        case 4:
			RETRO1.count++;
			RETRO1.prev = RETRO1.curr;
			RETRO1.curr = 1000*RETRO1.count;
			RETRO1.filter[MAINFILTERINDEX] = RETRO1.curr-RETRO1.prev;
			RETRO2.count++;
			RETRO2.prev = RETRO2.curr;
			RETRO2.curr = 1000*RETRO2.count;
			RETRO2.filter[MAINFILTERINDEX] = RETRO2.curr-RETRO2.prev;		
			RETRO3.count++;
			RETRO3.prev = RETRO3.curr;
			RETRO3.curr = 1000*RETRO3.count;
			RETRO3.filter[MAINFILTERINDEX] = RETRO3.curr-RETRO3.prev;
			break;

        default:
			printf("Not a valid retro. Choose 1-3.\r\n");
	}
}

// Determines the correct retro count. Votes on the counts seen on all retros.
// Which ever count has >=2 votes is determined to be correct.
int getStripCount(int *badRetro) {
	int actualCount = 0, agree1 = 0, agree2 = 0, agree3 = 0;
	//printf( "Retro 1 Count: %lu\r\n", RETRO1.count);
	//printf( "Retro 2 Count: %lu\r\n", RETRO2.count);
	//printf( "Retro 3 Count: %lu\r\n", RETRO3.count);
	
	if(RETRO1.count == RETRO2.count) {
		agree1++; agree2++;
	}
	if(RETRO1.count == RETRO3.count) {
		agree1++; agree3++;
	}
	if(RETRO2.count == RETRO3.count) {
		agree2++; agree3++;
	}
	if(agree1 >= 1) {
		actualCount = RETRO1.count;
	} else {
		*badRetro = 1;
		if(agree2 >= 1) {
			actualCount = RETRO2.count;
		} else if(agree3 >= 1) {
			actualCount = RETRO3.count;
		} else {
			// TODO NO retros agree... compare to estimated position and guess?

		}
	}	
	if(agree2 == 0) *badRetro = 2;
	if(agree3 == 0) *badRetro = 3;

	// Reset counts to the accurate amount. Useful in case a retro misses
	// Puts them all back on the same page.
	RETRO1.count = actualCount;
	RETRO2.count = actualCount;
	RETRO3.count = actualCount;

    /* Data is now fresh */
    freshness = FRESH; 


    return actualCount;
}

// Return 1 on success, 0 on failure. Updates global velocity.
// sets pos in cm
// sets vel in cm/s
// sets acc in cm/s^2
int getTelemetry(int *pos, int *vel, int *acc) {
	int numStrips, badRetro = 0; 
	uint32_t diff = 0; // Time between tape strips in ms
	// We are getting strip counts, getting a new one would throw it off
	__disable_irq(); 
	numStrips = getStripCount(&badRetro);	

	// FOR VEL: Average timestamps of all 3 retros
	// FOR ACC: Get difference for the previous count. Averaging again. 
	// Then by using two counts, time for strip n and time for strip n-1, calc accel
	diff += RETRO1.filter[MAINFILTERINDEX]; 
	diff += RETRO2.filter[MAINFILTERINDEX];
	diff += RETRO3.filter[MAINFILTERINDEX];

	switch (badRetro) {
		case 0:
			diff /= 3;
			break;
		case 1: 
			diff -= RETRO1.filter[MAINFILTERINDEX];
			diff /= 2;
			break;
		case 2:
			diff -= RETRO2.filter[MAINFILTERINDEX];
			diff /= 2;
			break;
		case 3:
			diff -= RETRO3.filter[MAINFILTERINDEX];
			diff /= 2;
			break;
		default:
			printf("No retros agree on counts\r\n");
			diff = -1;
			return 0;
	}
	
	__enable_irq();

	*pos = numStrips*CM_PER_STRIP;
	*vel = 0;
	if(diff <= 0) return 1;	// No strips read
	*vel = (1000 * CM_PER_STRIP) / diff; // dx/dt
	*acc = (tape_vel - *vel) / diff; //dv/dt 
	tape_vel = *vel;
	//printf("Vel: %dcm/s\t# Strip: %d\r\n", velocity, numStrips);

	return tape_vel;
}

void unitTest(void) {
	int tPos, tVel, tAcc;	
	int i;
	// Nominal case, all 3 strips read
	for( i = 0; i < 10; i++) {
		incVel(1); incVel(2); incVel(3);
		getTelemetry(&tPos, &tVel, &tAcc);
		if(tVel != CM_PER_STRIP) printf("Failed nominal case %d\r\n", tVel);
	}	
	
	// Pass two retros in one read	
	incVel(1); incVel(1); incVel(2); incVel(2); incVel(3); incVel(3);
	tVel = getTelemetry(&tPos, &tVel, &tAcc);
	if(tVel != CM_PER_STRIP) printf("Failed 2 strip case %d\r\n", tVel);

	// One retro missed	
	incVel(1); incVel(3);
	getTelemetry(&tPos, &tVel, &tAcc);
	if(tVel != CM_PER_STRIP) printf("FAILED: only 2 retro case %d\r\n", tVel);
	
	// Two retros missed
	incVel(1);
	getTelemetry(&tPos, &tVel, &tAcc);
	if(tVel != CM_PER_STRIP) printf("FAILED:\r\n");
}

// Detects bad retro by confirming counts amoung all retros.
// TODO What happens if a retro is bad? ignore it for rest of run?
void badRetro(void){
	if (RETRO1.count == RETRO2.count) {
	if (mainRetro != &RETRO1) printf("main retro now retro 1\r\n");
		mainRetro = &RETRO1;
	} else if (RETRO2.count == RETRO3.count) {
	if (mainRetro != &RETRO1) printf("main retro now retro 2\r\n");
		mainRetro = &RETRO2;
	} else if (RETRO1.count == RETRO3.count) {
		if (mainRetro != &RETRO1) printf("main retro now retro 1\r\n");
		mainRetro = &RETRO1;
	}
}


