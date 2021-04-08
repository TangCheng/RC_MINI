#include <8052.h>

#include "datatype.h"
#include "rf_comm.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"
#include "utils/utils.h"

enum State {
    INIT,
    PAIRING,
    RECEIVING
};

static byte buffer[8];
static enum State state = INIT;

void main()
{
    byte throttle = 0;
    byte steering = 0;
    LedUIInit();
    CommunicationInit();
    SysTickInit();
    RegisterTickProc(LedUITickProc);
    StartTick();
    state = PAIRING;
    PairWithController();
    while (true) {
        switch (state) {
        case PAIRING:
            if (PairWithController() == true) {
                state = RECEIVING;
            }
            break;
        case RECEIVING:
            if (ReceiveControlData(&throttle, &steering) == true) {
                buffer[0] = HUNDREDS_PLACE_CHAR(throttle);
                buffer[1] = TENS_PLACE_CHAR(throttle);
                buffer[2] = ONES_PLACE_CHAR(throttle);
                buffer[4] = HUNDREDS_PLACE_CHAR(steering);
                buffer[5] = TENS_PLACE_CHAR(steering);
                buffer[6] = ONES_PLACE_CHAR(steering);
                LedUIDisplay(buffer);
            }
            break;
        default:
            break;
        }
        delay(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
