#include <8052.h>

#include "common.h"
#include "datatype.h"
#include "drv/pcf8591.h"
#include "rf_comm.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte buffer[8];

void main()
{
    byte throttle = 0;
    byte steering = 0;
    LedUIInit();
    LedUIDisplay("12345678");
    RegisterTickProc(LedUITickProc);
    InitSysTick();
    StartTick();
    PairWithReceiver();
    while (1) {
        throttle = Pcf8591AdConversion(0);
        buffer[0] = (throttle / 100) + '0';
        buffer[1] = ((throttle % 100) / 10) + '0';
        buffer[2] = (throttle % 10) + '0';
        steering = Pcf8591AdConversion(1);
        buffer[4] = (steering / 100) + '0';
        buffer[5] = ((steering % 100) / 10) + '0';
        buffer[6] = (steering % 10) + '0';
        LedUIDisplay(buffer);
        SendControlData(throttle, steering);
        delay(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
