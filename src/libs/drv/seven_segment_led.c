#include "drv/seven_segment_led.h"

#include <8052.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEGMENT P0

// 共阴数码管显示字符0-F
byte __code segment[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void SevenSegmentLedDisplay(byte hex)
{
    if (hex >= '0' && hex <= '9') {
        hex -= '0';
    } else if (hex >= 'A' && hex <= 'F') {
        hex -= 'A';
    } else {
        SEGMENT = 0;
        return;
    }
    SEGMENT = segment[hex % 16];
}

#ifdef __cplusplus
}
#endif
