#include <8052.h>

#include "datatype.h"
#include "drv/pca9685.h"
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
    RegisterTickProc(0, LedUITickProc);
    StartTick();
    state = PAIRING;
    PairWithController();
    Pca9685Init();
    Pca9685SetPwmFrequency(50);
    Pca9685SetPin(7, 0);
    while (true) {
        switch (state) {
        case PAIRING:
            if (PairWithController() == true) {
                state = RECEIVING;
            }
            break;
        case RECEIVING:
            if (ReceiveControlData(&throttle, &steering) == true) {
                buffer[0] = ' ';
                buffer[1] = HUNDREDS_PLACE_CHAR(throttle);
                buffer[2] = TENS_PLACE_CHAR(throttle);
                buffer[3] = ONES_PLACE_CHAR(throttle);
                buffer[4] = ' ';
                buffer[5] = HUNDREDS_PLACE_CHAR(steering);
                buffer[6] = TENS_PLACE_CHAR(steering);
                buffer[7] = ONES_PLACE_CHAR(steering);
                LedUIDisplay(buffer);
                if (throttle <= 140) {
                    byte data = 140 - throttle;
                    Pca9685SetPin(7,  (data << 3) + (data << 1));
                }
                Pca9685SetPin(16, steering + 157);
            }
            break;
        default:
            break;
        }
        DelayMs(1);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
