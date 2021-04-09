#include "bus/i2c.h"

#include <8052.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCL P2_1
#define SDA P2_0
#define NOP __asm nop; nop __endasm

void I2cStart()
{
    SDA = 1; /* 发送起始条件的数据信号 */
    NOP;
    SCL = 1;
    NOP; /* 起始条件建立时间大于4.7us,延时 */
    NOP;
    NOP;
    NOP;
    NOP;
    SDA = 0; /* 发送起始信号 */
    NOP;     /* 起始条件锁定时间大于4μs */
    NOP;
    NOP;
    NOP;
    NOP;
    SCL = 0; /* 钳住I2C总线，准备发送或接收数据 */
    NOP;
    NOP;
}

void I2cStop()
{
    SDA = 0; /* 发送结束条件的数据信号 */
    NOP;     /* 发送结束条件的时钟信号 */
    SCL = 1; /* 结束条件建立时间大于4μs */
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    SDA = 1; /* 发送I2C总线结束信号 */
    NOP;
    NOP;
    NOP;
    NOP;
}

bool I2cSendByte(byte data)
{
    byte i = 0;
    bool ack = true;

    for (i = 0; i < 8; i++) /* 要传送的数据长度为8位 */
    {
        SDA = ((data << i) & 0x80) == 0 ? 0 : 1;
        NOP;
        SCL = 1; /* 置时钟线为高，通知被控器开始接收数据位 */
        NOP;
        NOP; /* 保证时钟高电平周期大于4μs */
        NOP;
        NOP;
        NOP;
        SCL = 0;
    }

    NOP;
    NOP;
    SDA = 1; /* 8位发送完后释放数据线，准备接收应答位 */
    NOP;
    NOP;
    SCL = 1;
    NOP;
    NOP;
    NOP;
    if (SDA == 1) {
        ack = false;
    } else {
        ack = true; /* 判断是否接收到应答信号 */
    }
    SCL = 0;
    NOP;
    NOP;

    return ack;
}

byte I2cReceiveByte()
{
    byte data = 0;
    byte i = 0;
    SDA = 1; /* 置数据线为输入方式 */
    for (i = 0; i < 8; i++) {
        NOP;
        SCL = 0; /* 置时钟线为低，准备接收数据位 */
        NOP;
        NOP; /* 时钟低电平周期大于4.7μs */
        NOP;
        NOP;
        NOP;
        SCL = 1; /* 置时钟线为高使数据线上数据有效 */
        NOP;
        NOP;
        data <<= 1;
        data += SDA; /* 读数据位,接收的数据位放入retc中 */
        NOP;
        NOP;
    }
    SCL = 0;
    NOP;
    NOP;
    return data;
}

void I2cAcknowledge(bool ack)
{
    if (ack == true) {
        SDA = 0; /* 在此发出应答或非应答信号 */
    } else {
        SDA = 1; /* 0为发出应答，1为非应答信号 */
    }
    NOP;
    NOP;
    NOP;
    SCL = 1;
    NOP;
    NOP; /* 时钟低电平周期大于4μs */
    NOP;
    NOP;
    NOP;
    SCL = 0; /* 清时钟线，钳住I2C总线以便继续接收 */
    NOP;
    NOP;
}

#ifdef __cplusplus
}
#endif
