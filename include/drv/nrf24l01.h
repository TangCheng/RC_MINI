#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void Nrf24l01Init(void);
void Nrf24l01Test(void);
void Nrf24l01Send(void);

#ifdef __cplusplus
}
#endif

#endif /* __NRF24L01_H__ */