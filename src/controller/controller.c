#include <8052.h>

#include "utils/delay.h"
#include "drv/seven_segment_led.h"
#include "sys/tick.h"

void main()
{
    SevenSegmentLedDisplay(0x07);
    RegisterTickProc(SevenSegmentLedTickProc);
    InitSysTick();
    StartTick();
    while (1) {
        // P1 = 0xAA;
        delay(250);
        // P1 = 0x55;
        delay(250);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
