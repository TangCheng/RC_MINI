#include "delay.h"

#ifdef __cplusplus
extern "C" {
#endif

// @12.000MHz
inline void delay1ms()	{
	byte i = 1;
    byte j = 110;
	do {
		while (--j);
	} while (--i);
}

void delay(word milliseconds) {
    while (milliseconds--) {
	    delay1ms();
    }
}

#ifdef __cplusplus
}
#endif
