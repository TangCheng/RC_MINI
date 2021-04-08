#include "rf_comm.h"

#include <8052.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

static byte buffer[PAYLOAD_LENGTH];
static byte __code address[5] = {'M', 'I', 'N', 'I', 'V'};

void PairWithReceiver()
{
    byte n = 0;
    byte connecting = 1;
    struct Payload payload;

    Nrf24l01Init();
    Nrf24l01Size(PAYLOAD_LENGTH);
    Nrf24l01PairMode();
    payload.header = PAYLOAD_NEGOTIATION_HEADER;
    for (n = 0; n < 5; n++) {
        payload.address[n] = address[n];
    }
    while (connecting) {
        // 把对频信息发给接收机，若收到回复表明通信成功，收不到继续发送
        Nrf24l01ChangeTransceiverMode(TX_MODE);
        Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
        delay(1);
        Nrf24l01ChangeTransceiverMode(RX_MODE);

        while (1) {
            delay(1);
            if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == PAYLOAD_LENGTH) {
                if (payload.header == PAYLOAD_RECEIVER_ACK_HEADER) {
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
    Nrf24l01ChangeTransceiverAddress(TX_MODE, address, sizeof(address));
    Nrf24l01ChangeTransceiverAddress(RX_MODE, address, sizeof(address));
    BuzzerBeep(250);
}

void SendControlData(byte throttle, byte steering)
{
    struct Payload payload;
    payload.header = PAYLOAD_CONTROL_DATA_HEADER;
    payload.throttle = throttle;
    payload.steering = steering;
    Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
}

#ifdef __cplusplus
}
#endif
