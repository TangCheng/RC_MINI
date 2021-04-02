#include <8052.h>

#include "utils/delay.h"
#include "ui/led_ui.h"
#include "sys/tick.h"
#include "drv/nrf24l01.h"

void main()
{
    Nrf24l01Init();
    LedUIDisplay("1 34");
    RegisterTickProc(LedUITickProc);
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
