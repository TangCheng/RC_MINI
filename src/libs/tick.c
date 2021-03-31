#include "tick.h"

#include <8052.h>
#include <stdio.h>

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TICK 5  // milliseconds
// @12MHz, 12T, 2^16 - tick * 1000
#define TIMER_VALUE ((1L << 16) - (TICK * 1000L))
#define TIMER_HIGH_VALUE (byte)(TIMER_VALUE >> 8)
#define TIMER_LOW_VALUE (byte)(TIMER_VALUE & 0x00FF)

inline void TickTimerProc()
{
    TH0 = TIMER_HIGH_VALUE;
    TL0 = TIMER_LOW_VALUE;
    static byte value = 0x01;
    P2 = (value << 4) | 0x0F;
    value <<= 1;
    if (value == 0x10) {
        value = 0x01;
    }
}

void InitSysTick()
{
    // Timer1, timer mode, work mode 1
    TMOD = 0x01;
    TH0 = TIMER_HIGH_VALUE;
    TL0 = TIMER_LOW_VALUE;
    // Enable timer interrupt
    ET0 = 1;
    EA = 1;
}

int RegisterTickProc(TickProc tickProc)
{
    if (tickProc != NULL) {
        tickProc();
    }
    return 0;
}

int UnregisterTickProc(TickProc tickProc)
{
    if (tickProc != NULL) {
        tickProc();
    }
    return 0;
}

void StartTick()
{
    TR0 = 1;
}

void StopTick()
{
    TR0 = 0;
}

#ifdef __cplusplus
}
#endif
