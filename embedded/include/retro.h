#ifndef _RETRO__H__
#define _RETRO__H__

#include "exti.h"

timeStamp * mainRetro;

#define RETRO1 		interLine[0]
#define RETRO2 		interLine[1]
#define RETRO3		interLine[5]

#define MAINFILTERINDEX 	(mainRetro->count % AVERAGE_SIZE)

#define MAIN_INTERVAL       (mainRetro->filter[MAINFILTERINDEX])

void initRetro(void);
int getVelocity(void);
#endif /* _RETR0_H*/
