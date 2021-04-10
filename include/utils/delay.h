#ifndef __DELAY_H__
#define __DELAY_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void DelayMs(word milliseconds);
inline void DelayUs(word useconds);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H__ */