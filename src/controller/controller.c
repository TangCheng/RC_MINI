#include <8052.h>
#include <string.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "drv/pcf8591.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte buffer[PAYLOAD_LENGTH];
static struct PairPayload pairPayload;
static struct DataPayload dataPayload;

void PairWithReceiver()
{
    byte n = 0;
    byte connecting = 1;
    pairPayload.header = PAYLOAD_NEGOTIATION_HEADER;

    Nrf24l01Size(PAYLOAD_LENGTH);
    Nrf24l01PairMode();
    while (connecting) {
        // 把对频信息发给接收机，若收到回复表明通信成功，收不到继续发送
        Nrf24l01ChangeTransceiverMode(TX_MODE);
        Nrf24l01BufferWrite((byte *)&pairPayload, sizeof(pairPayload));
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
    Nrf24l01ChangeTransceiverAddress(TX_MODE, pairPayload.address, sizeof(pairPayload.address));
    Nrf24l01ChangeTransceiverAddress(RX_MODE, pairPayload.address, sizeof(pairPayload.address));
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
    pairPayload.address[0] = 'M';
    pairPayload.address[1] = 'I';
    pairPayload.address[2] = 'N';
    pairPayload.address[3] = 'I';
    pairPayload.address[4] = 'V';
    dataPayload.header = PAYLOAD_DATA_HEADER;
    PairWithReceiver();
    while (1) {
        data = Pcf8591AdConversion(0);
        buffer[0] = (data / 100) + '0';
        buffer[1] = ((data % 100) / 10) + '0';
        buffer[2] = (data % 10) + '0';
        dataPayload.throttle = data;
        data = Pcf8591AdConversion(1);
        buffer[4] = (data / 100) + '0';
        buffer[5] = ((data % 100) / 10) + '0';
        buffer[6] = (data % 10) + '0';
        LedUIDisplay(buffer);
        dataPayload.steering = data;
        Nrf24l01BufferWrite((byte *)&dataPayload, sizeof(dataPayload));
        delay(5);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
