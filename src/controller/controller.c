#include <8052.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "drv/pcf8591.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte buffer[PAYLOAD_LENGTH];
static struct Payload payload;

void PairWithReceiver()
{
    byte n = 0;
    byte connecting = 1;

    Nrf24l01Size(PAYLOAD_LENGTH);
    Nrf24l01PairMode();
    while (connecting) {
        // 把对频信息发给接收机，若收到回复表明通信成功，收不到继续发送
        Nrf24l01ChangeTransceiverMode(TX_MODE);
        Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
        delay(1);
        Nrf24l01ChangeTransceiverMode(RX_MODE);

        while (1) {
            delay(1);
            if (Nrf24l01BufferRead(buffer, sizeof(buffer)) == PAYLOAD_LENGTH) {
                if (buffer[0] == 'O' && buffer[1] == 'K') {
                    connecting = 0;
                }
            }

            n++;
            if (n > 50) {
                n = 0;
                break;
            }
        }
    }

    Nrf24l01ChangeTransceiverMode(TX_MODE);
    Nrf24l01WorkMode();
    Nrf24l01ChangeTransceiverAddress(TX_MODE, payload.address, sizeof(payload.address));
    Nrf24l01ChangeTransceiverAddress(RX_MODE, payload.address, sizeof(payload.address));
    BuzzerBeep(250);
}

void main()
{
    byte data = 0;
    LedUIInit();
    LedUIDisplay("12345678");
    RegisterTickProc(LedUITickProc);
    InitSysTick();
    StartTick();
    Nrf24l01Init();
    payload.header = PAYLOAD_NEGOTIATION_HEADER;
    payload.address[0] = 'M';
    payload.address[1] = 'I';
    payload.address[2] = 'N';
    payload.address[3] = 'I';
    payload.address[4] = 'V';
    PairWithReceiver();
    payload.header = PAYLOAD_CONTROL_DATA_HEADER;
    while (1) {
        data = Pcf8591AdConversion(0);
        buffer[0] = (data / 100) + '0';
        buffer[1] = ((data % 100) / 10) + '0';
        buffer[2] = (data % 10) + '0';
        payload.throttle = data;
        data = Pcf8591AdConversion(1);
        buffer[4] = (data / 100) + '0';
        buffer[5] = ((data % 100) / 10) + '0';
        buffer[6] = (data % 10) + '0';
        LedUIDisplay(buffer);
        payload.steering = data;
        Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
        delay(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
