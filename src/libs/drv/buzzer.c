#include "drv/buzzer.h"

#include <8052.h>

#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUZZER_CTRL P2_3
#define BUZZER_ON 0
#define BUZZER_OFF 1

void BuzzerBeep(byte beepPeriod)
{
    BUZZER_CTRL = BUZZER_ON;
    DelayMs(beepPeriod);
    BUZZER_CTRL = BUZZER_OFF;
}

#ifdef __cplusplus
}
#endif
