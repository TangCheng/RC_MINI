#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void Nrf24l01Init(void);
void Nrf24l01PairMode(void);
void Nrf24l01WorkMode(void);
void Nrf24l01TxMode(void);
void Nrf24l01TxAddress(byte *data);
void Nrf24l01RxMode(void);
void Nrf24l01RxAddress(byte *data);
int Nrf24l01BufferRead(byte *buffer, byte length);
void Nrf24l01BufferWrite(byte *buffer, byte length);
void Nrf24l01Send(void);

#ifdef __cplusplus
}
#endif

#endif /* __NRF24L01_H__ */