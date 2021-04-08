#include "drv/pcf8591.h"

#include "bus/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCF8591_WRITE_ADDR 0x90
#define PCF8591_READ_ADDR (PCF8591_WRITE_ADDR | 0x01)

byte Pcf8591AdConversion(byte channel)
{
    bool ack = false;
    byte data;

    I2cStart();                             // 启动总线
    ack = I2cSendByte(PCF8591_WRITE_ADDR);  // 发送器件地址
    if (ack == false) {
        return 0;
    }
    ack = I2cSendByte(0x40 | channel);  // 发送控制字节
    if (ack == false) {
        return 0;
    }
    I2cStop();  // 结束总线

    I2cStart();                            // 启动总线
    ack = I2cSendByte(PCF8591_READ_ADDR);  // 发送器件地址
    if (ack == false) {
        return 0;
    }
    data = I2cReceiveByte();  // 读取数据0

    I2cAcknowledge(false);  // 发送非应答信号
    I2cStop();          // 结束总线
    return data;
}

#ifdef __cplusplus
}
#endif
