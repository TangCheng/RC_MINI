#ifndef __TICK_H__
#define __TICK_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TickProc)(void);

void SysTickInit();
bool RegisterTickProc(TickProc tickProc);
bool UnregisterTickProc(TickProc tickProc);
void StartTick();
void StopTick();
inline void TickTimerProc(void);

#ifdef __cplusplus
}
#endif

#endif /* __TICK_H__ */