#ifndef __TICK_H__
#define __TICK_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TICK 1  // milliseconds

typedef void (*TickProc)(void);

void SysTickInit();
bool RegisterTickProc(byte index, TickProc tickProc);
bool UnregisterTickProc(byte index);
void StartTick();
void StopTick();
inline void TickTimerProc(void);

#ifdef __cplusplus
}
#endif

#endif /* __TICK_H__ */