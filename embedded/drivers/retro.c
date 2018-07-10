#include "stdio.h"
#include "retro.h"
#include "badgerloop.h"

void initRetro(void) {
	mainRetro = &RETRO1;
}

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

int getVelocity(void) {
	int velocity;
	//badRetro();
	__disable_irq();
	if (!MAIN_INTERVAL) return 0;
	velocity = (1000 * CM_PER_STRIP) / MAIN_INTERVAL;
	__enable_irq();
	return velocity;
}
