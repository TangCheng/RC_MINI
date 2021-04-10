#include <8052.h>

#include "control_signal_process.h"
#include "datatype.h"
#include "rf_comm.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

enum State {
    INIT,
    PAIRING,
    CONTROLING
};

static enum State state = INIT;

void main()
{
    LedUIInit();
    CommunicationInit();
    SysTickInit();
    RegisterTickProc(0, LedUITickProc);
    StartTick();
    state = PAIRING;
    LedUIDisplay("12345678");
    while (true) {
        switch (state) {
        case PAIRING:
            if (PairWithReceiver() == true) {
                state = CONTROLING;
                RegisterTickProc(1, ControlSignalCollectionTickProc);
            }
            break;
        case CONTROLING:
            break;
        default:
            break;
        }
        DelayMs(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
