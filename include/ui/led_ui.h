#ifndef __LED_UI_H__
#define __LED_UI_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void LedUITickProc();
void LedUIInit();
void LedUIDisplay(byte *string);

#ifdef __cplusplus
}
#endif

#endif /* __LED_UI_H__ */