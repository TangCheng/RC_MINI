#include "control_signal_process.h"

#include "drv/pcf8591.h"
#include "rf_comm.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONTROL_SIGNAL_COLLECTION_PERIOD 20  // milliseconds
#define CONTROL_SIGNAL_THROTTLE_ADC_CHANNEL 0
#define CONTROL_SIGNAL_STEERING_ADC_CHANNEL 1

static inline void CollectControlSignal()
{
    byte throttle = 0;
    byte steering = 0;
    byte buffer[8];
    throttle = Pcf8591AdConversion(CONTROL_SIGNAL_THROTTLE_ADC_CHANNEL);
    buffer[0] = ' ';
    buffer[1] = HUNDREDS_PLACE_CHAR(throttle);
    buffer[2] = TENS_PLACE_CHAR(throttle);
    buffer[3] = ONES_PLACE_CHAR(throttle);
    steering = Pcf8591AdConversion(CONTROL_SIGNAL_STEERING_ADC_CHANNEL);
    buffer[4] = ' ';
    buffer[5] = HUNDREDS_PLACE_CHAR(steering);
    buffer[6] = TENS_PLACE_CHAR(steering);
    buffer[7] = ONES_PLACE_CHAR(steering);
    LedUIDisplay(buffer);
    SendControlData(throttle, steering);
}

void ControlSignalCollectionTickProc()
{
    static byte count = 0;
    if (++count >= 20) {
        count = 0;
        CollectControlSignal();
    }
}

#ifdef __cplusplus
}
#endif
