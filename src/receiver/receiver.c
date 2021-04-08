#include <8052.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte buffer[PAYLOAD_LENGTH] = {PAYLOAD_RECEIVER_ACK_HEADER};
static struct Payload payload;

void PairWithController()
{
    Nrf24l01Size(PAYLOAD_LENGTH);
    Nrf24l01PairMode();
    Nrf24l01ChangeTransceiverMode(RX_MODE);
    while (1) {
        // 读取接收数据
        if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == PAYLOAD_LENGTH) {
            if (payload.header == PAYLOAD_NEGOTIATION_HEADER) {
                break;
            }
        }
    }

    Nrf24l01ChangeTransceiverMode(TX_MODE);
    Nrf24l01BufferWrite(buffer, sizeof(buffer));
    delay(1);

    Nrf24l01ChangeTransceiverMode(RX_MODE);
    Nrf24l01WorkMode();
    Nrf24l01ChangeTransceiverAddress(TX_MODE, payload.address, sizeof(payload.address));
    Nrf24l01ChangeTransceiverAddress(RX_MODE, payload.address, sizeof(payload.address));
    BuzzerBeep(250);
}

void main()
{
    LedUIInit();
    LedUIDisplay("90ABCDEF");
    RegisterTickProc(LedUITickProc);
    InitSysTick();
    StartTick();
    Nrf24l01Init();
    PairWithController();
    while (1) {
        if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == PAYLOAD_LENGTH) {
            if (payload.header == PAYLOAD_CONTROL_DATA_HEADER) {
                buffer[0] = (payload.throttle / 100) + '0';
                buffer[1] = ((payload.throttle % 100) / 10) + '0';
                buffer[2] = (payload.throttle % 10) + '0';
                buffer[4] = (payload.steering / 100) + '0';
                buffer[5] = ((payload.steering % 100) / 10) + '0';
                buffer[6] = (payload.steering % 10) + '0';
                LedUIDisplay(buffer);
            }
        }
        delay(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
