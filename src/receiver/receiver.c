#include <8052.h>
#include <string.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte buffer[PAYLOAD_LENGTH];
static struct PairPayload pairPayload;
static struct DataPayload dataPayload;

void PairWithController()
{
    Nrf24l01Size(PAYLOAD_LENGTH);
    Nrf24l01PairMode();
    Nrf24l01ChangeTransceiverMode(RX_MODE);
    while (1) {
        // 读取接收数据
        if (Nrf24l01BufferRead((byte *)&pairPayload, sizeof(pairPayload)) == PAYLOAD_LENGTH) {
            if (pairPayload.header == PAYLOAD_NEGOTIATION_HEADER) {
                break;
            }
        }
    }

    strcpy(buffer, "OK");
    Nrf24l01ChangeTransceiverMode(TX_MODE);
    Nrf24l01BufferWrite(buffer, sizeof(buffer));
    delay(1);

    Nrf24l01ChangeTransceiverMode(RX_MODE);
    Nrf24l01WorkMode();
    Nrf24l01ChangeTransceiverAddress(TX_MODE, pairPayload.address, sizeof(pairPayload.address));
    Nrf24l01ChangeTransceiverAddress(RX_MODE, pairPayload.address, sizeof(pairPayload.address));
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
        if (Nrf24l01BufferRead((byte *)&dataPayload, sizeof(dataPayload)) == PAYLOAD_LENGTH) {
            if (dataPayload.header == PAYLOAD_DATA_HEADER) {
                buffer[0] = (dataPayload.throttle / 100) + '0';
                buffer[1] = ((dataPayload.throttle % 100) / 10) + '0';
                buffer[2] = (dataPayload.throttle % 10) + '0';
                buffer[4] = (dataPayload.steering / 100) + '0';
                buffer[5] = ((dataPayload.steering % 100) / 10) + '0';
                buffer[6] = (dataPayload.steering % 10) + '0';
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
