#include "ui/led_ui.h"

#include <8052.h>
#include <string.h>

#include "drv/seven_segment_led.h"
#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LED_SELECT P0
#define LED_SELECT_NONE 0xFF
#define LED_SELECT_LATCH P2_7
#define LED_SELECT_LATCH_ENABLE 1
#define LED_SELECT_LATCH_DISABLE 0
#define LED_AMOUNT 8

byte content[LED_AMOUNT] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
byte index = 0;

void LedUITickProc()
{
    LED_SELECT = LED_SELECT_NONE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_ENABLE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    SevenSegmentLedDisplay(content[index]);
    LED_SELECT = ~(0x01 << index);
    LED_SELECT_LATCH = LED_SELECT_LATCH_ENABLE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    index++;
    index %= LED_AMOUNT;
}

void LedUIInit()
{
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    SevenSegmentLedInit();
}

void LedUIDisplay(byte *string)
{
    memcpy(content, string, LED_AMOUNT);
}

#ifdef __cplusplus
}
#endif
