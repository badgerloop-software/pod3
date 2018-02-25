#include "system.h"
#include "board.h"

#define BLINK_INTERVAL	250

int main(void) {

	io_init();

	while (1) {
		blink_handler(BLINK_INTERVAL);
	}

	return 0;
}
