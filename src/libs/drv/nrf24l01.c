#include "drv/nrf24l01.h"

#include <8052.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RST P1_0
#define SCK P1_1
#define MIS P1_2
#define MOS P1_4
#define CS P1_3
#define IRQ P1_5

static byte __code powerTable[] = {
    0x21,  // -18db
    0x23,  // -12db
    0x25,  // -6db
    0x27   // 0db
};

static byte address[5];

static byte SPI(byte data)
{
    byte i = 0;
    for (i = 0; i < 8; i++) {
        MIS = (data & 0x80);
        SCK = 1;
        data <<= 1;
        data |= MOS;
        SCK = 0;
    }
    return data;
}

static void Nrf24l01RegisterWrite(byte address, byte data)
{
    CS = 0;
    SPI(0x20 + address);
    SPI(data);
    CS = 1;
}

static byte Nrf24l01RegisterRead(byte address)
{
    byte data;
    CS = 0;
    SPI(address);
    data = SPI(0);
    CS = 1;
    return data;
}

static void Nrf24l01TxMode()
{
    RST = 0;
    Nrf24l01RegisterWrite(0x00, 0x0a);
    RST = 1;
}

static void Nrf24l01RxMode()
{
    RST = 0;
    Nrf24l01RegisterWrite(0x00, 0x3b);  //CRC,8 bit,Power on,RX
    RST = 1;
}

static void Nrf24l01Channel(byte channel)
{
    RST = 0;
    Nrf24l01RegisterWrite(0x05, channel);
    RST = 1;
}

static void Nrf24l01Power(byte power)  //发射功率设置
{
    RST = 0;
    power %= sizeof(powerTable);
    Nrf24l01RegisterWrite(0x06, powerTable[power]);
    RST = 1;
}

static void Nrf24l01Size(byte size)
{
    RST = 0;
    Nrf24l01RegisterWrite(0x11, size);
    RST = 1;
}

static void Nrf24l01TxAddress(byte *data)
{
    CS = 0;
    SPI(0x20 + 0x10);
    SPI(data[0]);
    SPI(data[1]);
    SPI(data[2]);
    SPI(data[3]);
    SPI(data[4]);
    CS = 1;
}

static void Nrf24l01RxAddress(byte *data)
{
    CS = 0;
    SPI(0x20 + 0x0a);
    SPI(data[0]);
    SPI(data[1]);
    SPI(data[2]);
    SPI(data[3]);
    SPI(data[4]);
    CS = 1;
}

void Nrf24l01Init(void)
{
    RST = 0;
    SCK = 0;
    Nrf24l01RegisterWrite(0x01, 0x00);  //禁止 自动应答
    Nrf24l01RegisterWrite(0x02, 0x01);  //允许 P0信道
    Nrf24l01RegisterWrite(0x04, 0x00);  //禁止 自动重发
    Nrf24l01TxMode();
    Nrf24l01Channel(66);
    Nrf24l01Power(3);
    Nrf24l01Size(11);
    Nrf24l01TxAddress(address);
    Nrf24l01RxAddress(address);
}

void Nrf24l01Test(void)
{
}

void Nrf24l01Send(void)
{
}

#ifdef __cplusplus
}
#endif
