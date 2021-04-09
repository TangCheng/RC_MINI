#include "sys/tick.h"

#include <8052.h>
#include <stdio.h>

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TICK_PROC 5
// @12MHz, 6T, 2^16 - tick * 2000
#define TIMER_VALUE ((1L << 16) - ((TICK << 11) - (TICK << 5) - (TICK << 4)))
#define TIMER_HIGH_VALUE (byte)(TIMER_VALUE >> 8)
#define TIMER_LOW_VALUE (byte)(TIMER_VALUE & 0x00FF)

static TickProc tickProcs[MAX_TICK_PROC] = {
    NULL, NULL, NULL, NULL, NULL};

inline void TickTimerProc()
{
    byte i = 0;
    TH0 = TIMER_HIGH_VALUE;
    TL0 = TIMER_LOW_VALUE;
    for (i = 0; i < MAX_TICK_PROC; i++) {
        if (tickProcs[i] != NULL) {
            tickProcs[i]();
        }
    }
}

void SysTickInit()
{
    // Timer0, timer mode, work mode 1
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = TIMER_HIGH_VALUE;
    TL0 = TIMER_LOW_VALUE;
    // Enable timer interrupt
    ET0 = 1;
    EA = 1;
}

bool RegisterTickProc(byte index, TickProc tickProc)
{
    if (index >= MAX_TICK_PROC || tickProc == NULL) {
        return false;
    }
    tickProcs[index] = tickProc;
    return true;
}

bool UnregisterTickProc(byte index)
{
    if (index >= MAX_TICK_PROC) {
        return false;
    }
    tickProcs[index] = NULL;
    return true;
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
