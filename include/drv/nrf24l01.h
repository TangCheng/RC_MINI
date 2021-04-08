#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

enum Nrf24l01TransceiverMode {
    TX_MODE,
    RX_MODE
};

void Nrf24l01Init(void);
void Nrf24l01PairMode(void);
void Nrf24l01WorkMode(void);
void Nrf24l01ChangeTransceiverMode(enum Nr24l01TransceiverMode mode);
void Nrf24l01ChangeTransceiverAddress(enum Nrf24l01TransceiverMode mode,
                                      byte *address, byte length);
int Nrf24l01BufferRead(byte *buffer, byte length);
void Nrf24l01BufferWrite(byte *buffer, byte length);

#ifdef __cplusplus
}
#endif

#endif /* __NRF24L01_H__ */