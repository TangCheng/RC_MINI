#include "utils/delay.h"

#include <8052.h>

#ifdef __cplusplus
extern "C" {
#endif

// @12.000MHz 6T
static inline void delay1ms()
{
    byte i = 1;
    byte j = 220;
    do {
        while (--j) {
            ;
        }
    } while (--i);
}

static inline void delay1us()
{
    __asm nop
        nop
            __endasm;
}

inline void DelayMs(word milliseconds)
{
    do {
        delay1ms();
    } while (--milliseconds);
}

inline void DelayUs(word useconds)
{
    do {
        delay1us();
    } while (--useconds);
}

#ifdef __cplusplus
}
#endif
