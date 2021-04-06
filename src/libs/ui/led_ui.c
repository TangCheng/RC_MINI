#include "ui/led_ui.h"

#include <8052.h>
#include <string.h>

#include "drv/seven_segment_led.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LED_SELECT P2
#define LED_AMOUNT 4

byte content[LED_AMOUNT] = {' ', ' ', ' ', ' '};
byte index = 0;

void LedUITickProc()
{
    LED_SELECT = LED_SELECT & 0x0F;
    SevenSegmentLedDisplay(content[index]);
    LED_SELECT = (0x10 << index) | 0x0F & LED_SELECT;
    index++;
    index %= LED_AMOUNT;
}

void LedUIDisplay(byte *string)
{
    memcpy(content, string, LED_AMOUNT);
}

#ifdef __cplusplus
}
#endif
