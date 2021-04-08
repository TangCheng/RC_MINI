#ifndef __TICK_H__
#define __TICK_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TickProc)(void);

void SysTickInit();
int RegisterTickProc(TickProc tickProc);
int UnregisterTickProc(TickProc tickProc);
void StartTick();
void StopTick();
inline void TickTimerProc(void);

#ifdef __cplusplus
}
#endif

#endif /* __TICK_H__ */