#include "ui/led_ui.h"

#include <8052.h>

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

static byte content[LED_AMOUNT] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
static byte index = 0;

void LedUITickProc()
{
    LED_SELECT = LED_SELECT_NONE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_ENABLE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    SevenSegmentLedDisplay(content[index]);
    LED_SELECT = ~(0x01 << index);
    LED_SELECT_LATCH = LED_SELECT_LATCH_ENABLE;
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    if (++index >= LED_AMOUNT) {
        index = 0;
    }
}

void LedUIInit()
{
    LED_SELECT_LATCH = LED_SELECT_LATCH_DISABLE;
    SevenSegmentLedInit();
}

void LedUIDisplay(byte *string)
{
    byte i = 0;
    for (i = 0; i < LED_AMOUNT; i++) {
        content[i] = string[i];
    }
}

#ifdef __cplusplus
}
#endif
