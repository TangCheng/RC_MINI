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

static byte __code address[5] = {'M', 'I', 'N', 'I', 'V'};

void CommunicationInit()
{
    Nrf24l01Init();
    Nrf24l01Size(PAYLOAD_LENGTH);
}

bool PairWithReceiver()
{
    byte i = 0;
    struct Payload payload;

    Nrf24l01PairMode();
    payload.header = PAYLOAD_NEGOTIATION_HEADER;
    for (i = 0; i < 5; i++) {
        payload.address[i] = address[i];
    }

    Nrf24l01ChangeTransceiverMode(TX_MODE);
    Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
    DelayMs(1);
    Nrf24l01ChangeTransceiverMode(RX_MODE);
    DelayMs(100);
    if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == true) {
        if (payload.header == PAYLOAD_RECEIVER_ACK_HEADER) {
            Nrf24l01ChangeTransceiverMode(TX_MODE);
            Nrf24l01WorkMode();
            Nrf24l01ChangeTransceiverAddress(TX_MODE, address, sizeof(address));
            Nrf24l01ChangeTransceiverAddress(RX_MODE, address, sizeof(address));
            BuzzerBeep(250);
            return true;
        }
    }
    return false;
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
