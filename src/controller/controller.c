#include <8052.h>

#include "delay.h"
#include "seven_segment_led.h"
#include "tick.h"
#include "utils.h"

void main()
{
    SevenSegmentLedDisplay(0x07);
    RegisterTickProc(SevenSegmentLedTickProc);
    InitSysTick();
    StartTick();
    utils();
    delay(5000);
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
