// 
// 
// 

#include "IRTimer.h"
IRtimer::IRtimer() {
	reset();
}

void ICACHE_FLASH_ATTR IRtimer::reset() {
	start = micros();
}

uint32_t ICACHE_FLASH_ATTR IRtimer::elapsed() {
	uint32_t now = micros();
	if (start <= now)  // Check if the system timer has wrapped.
		return (now - start);  // No wrap.
	else
		return (0xFFFFFFFF - start + now);  // Has wrapped.
}



