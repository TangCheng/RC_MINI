#include "sys/tick.h"

#include <8052.h>
#include <stdio.h>

#include "datatype.h"
#include "utils/list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TICK_PROC 5
#define TICK 2  // milliseconds
// @12MHz, 12T, 2^16 - tick * 1000
#define TIMER_VALUE ((1L << 16) - (TICK * 1000L))
#define TIMER_HIGH_VALUE (byte)(TIMER_VALUE >> 8)
#define TIMER_LOW_VALUE (byte)(TIMER_VALUE & 0x00FF)

static List *tickProcs = NULL;
static byte tickProcCount = 0;

void CallTickProc(void **x, void *cl)
{
    if (x != NULL && *x != NULL) {
        TickProc tickProc = (TickProc)*x;
        tickProc();
    }
    cl = cl;
}

inline void TickTimerProc()
{
    TH0 = TIMER_HIGH_VALUE;
    TL0 = TIMER_LOW_VALUE;
    List_map(tickProcs, CallTickProc, NULL);
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

int RegisterTickProc(TickProc tickProc)
{
    if (tickProcCount >= MAX_TICK_PROC) {
        return -1;
    }

    if (tickProc == NULL) {
        return -1;
    }

    tickProcs = List_push(tickProcs, (void *)tickProc);
    tickProcCount++;
    return 0;
}

int UnregisterTickProc(TickProc tickProc)
{
    if (tickProc == NULL) {
        return -1;
    }

    tickProcs = List_remove(tickProcs, (void *)tickProc);
    tickProcCount--;
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
