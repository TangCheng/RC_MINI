#include "bus/i2c.h"

#include <8052.h>

#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCL P2_1
#define SDA P2_0

void I2cStart()
{
    SDA = 1; /* 发送起始条件的数据信号 */
    DelayUs(1);
    SCL = 1;
    DelayUs(5); /* 起始条件建立时间大于4.7us,延时 */
    SDA = 0; /* 发送起始信号 */
    DelayUs(5);     /* 起始条件锁定时间大于4μs */
    SCL = 0; /* 钳住I2C总线，准备发送或接收数据 */
    DelayUs(2);
}

void I2cStop()
{
    SDA = 0; /* 发送结束条件的数据信号 */
    DelayUs(1);     /* 发送结束条件的时钟信号 */
    SCL = 1; /* 结束条件建立时间大于4μs */
    DelayUs(5);
    SDA = 1; /* 发送I2C总线结束信号 */
    DelayUs(4);
}

bool I2cSendByte(byte data)
{
    byte i = 0;
    bool ack = true;

    for (i = 0; i < 8; i++) /* 要传送的数据长度为8位 */
    {
        SDA = ((data << i) & 0x80) == 0 ? 0 : 1;
        DelayUs(1);
        SCL = 1; /* 置时钟线为高，通知被控器开始接收数据位 */
        DelayUs(5); /* 保证时钟高电平周期大于4μs */
        SCL = 0;
    }

    DelayUs(2);
    SDA = 1; /* 8位发送完后释放数据线，准备接收应答位 */
    DelayUs(2);
    SCL = 1;
    DelayUs(3);
    if (SDA == 1) {
        ack = false;
    } else {
        ack = true; /* 判断是否接收到应答信号 */
    }
    SCL = 0;
    DelayUs(2);

    return ack;
}

byte I2cReceiveByte()
{
    byte data = 0;
    byte i = 0;
    SDA = 1; /* 置数据线为输入方式 */
    for (i = 0; i < 8; i++) {
        DelayUs(1);
        SCL = 0; /* 置时钟线为低，准备接收数据位 */
        DelayUs(5); /* 时钟低电平周期大于4.7μs */
        SCL = 1; /* 置时钟线为高使数据线上数据有效 */
        DelayUs(2);
        data <<= 1;
        data += SDA; /* 读数据位,接收的数据位放入data中 */
        DelayUs(2);
    }
    SCL = 0;
    DelayUs(2);
    return data;
}

void I2cAcknowledge(bool ack)
{
    if (ack == true) {
        SDA = 0; /* 在此发出应答或非应答信号 */
    } else {
        SDA = 1; /* 0为发出应答，1为非应答信号 */
    }
    DelayUs(3);
    SCL = 1;
    DelayUs(5); /* 时钟低电平周期大于4μs */
    SCL = 0; /* 清时钟线，钳住I2C总线以便继续接收 */
    DelayUs(2);
}

#ifdef __cplusplus
}
#endif
