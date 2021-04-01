#ifndef __SEVEN_SEGMENT_LED_H__
#define __SEVEN_SEGMENT_LED_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void SevenSegmentLedDisplay(byte hex);
void SevenSegmentLedTickProc();

#ifdef __cplusplus
}
#endif

#endif /* __SEVEN_SEGMENT_LED_H__ */