#ifndef __I2C_H__
#define __I2C_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void I2cStart();
void I2cStop();
int I2cSendByte(byte data);
byte I2cReceiveByte();
void I2cAcknowledge(byte ack);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */