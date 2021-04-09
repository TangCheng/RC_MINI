#include "rf_comm.h"

#include <8052.h>
#include <stdio.h>

#include "common.h"
#include "datatype.h"
#include "drv/buzzer.h"
#include "drv/nrf24l01.h"
#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

static byte address[5];

void CommunicationInit()
{
    Nrf24l01Init();
    Nrf24l01Size(PAYLOAD_LENGTH);
}

bool PairWithController()
{
    byte i = 0;
    struct Payload payload;

    Nrf24l01PairMode();
    Nrf24l01ChangeTransceiverMode(RX_MODE);
    if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == true) {
        if (payload.header == PAYLOAD_NEGOTIATION_HEADER) {
            for (i = 0; i < 5; i++) {
                address[i] = payload.address[i];
            }
            Nrf24l01ChangeTransceiverMode(TX_MODE);
            payload.header = PAYLOAD_RECEIVER_ACK_HEADER;
            Nrf24l01BufferWrite((byte *)&payload, PAYLOAD_LENGTH);
            delay(1);

            Nrf24l01ChangeTransceiverMode(RX_MODE);
            Nrf24l01WorkMode();
            Nrf24l01ChangeTransceiverAddress(TX_MODE, address, sizeof(address));
            Nrf24l01ChangeTransceiverAddress(RX_MODE, address, sizeof(address));
            BuzzerBeep(250);
            return true;
        }
    }
    return false;
}

bool ReceiveControlData(byte *throttle, byte *steering)
{
    struct Payload payload;
    if (throttle == NULL || steering == NULL) {
        return false;
    }
    if (Nrf24l01BufferRead((byte *)&payload, PAYLOAD_LENGTH) == true) {
        if (payload.header == PAYLOAD_CONTROL_DATA_HEADER) {
            *throttle = payload.throttle;
            *steering = payload.steering;
            return true;
        }
    }
    return false;
}

#ifdef __cplusplus
}
#endif
