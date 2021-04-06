#include "drv/nrf24l01.h"

#include <8052.h>

#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CE P1_0
#define SCK P1_1
#define MISO P1_2
#define MOSI P1_4
#define CSN P1_3
#define IRQ P1_5

#define COMMAND_READ_REGISTER 0x00
#define COMMAND_WRITE_REGISTER 0x20
#define COMMAND_READ_RX_PAYLOAD 0x61
#define COMMAND_WRITE_TX_PAYLOAD 0xA0
#define COMMAND_FLUSH_RX 0xE2
#define COMMAND_FLUSH_TX 0xE1
#define COMMAND_REUSE_TX_PAYLOAD 0xE3
#define COMMAND_NOP 0xFF

#define REGISTER_CONFIG 0x00
#define REGISTER_ENABLE_AUTO_ACK 0x01
#define REGISTER_ENABLE_RX_ADDRESS 0x02
#define REGISTER_SETUP_ADDRESS_WIDTH 0x03
#define REGISTER_SETUP_AUTO_RETRANS 0x04
#define REGISTER_RF_CHANNEL 0x05
#define REGISTER_RF_SETUP 0x06
#define REGISTER_STATUS 0x07
#define REGISTER_OBSERVE_TX 0x08
#define REGISTER_CARRIER_DETECT 0x09
#define REGISTER_RX_ADDRESS_P0 0x0A
#define REGISTER_RX_ADDRESS_P1 0x0B
#define REGISTER_RX_ADDRESS_P2 0x0C
#define REGISTER_RX_ADDRESS_P3 0x0D
#define REGISTER_RX_ADDRESS_P4 0x0E
#define REGISTER_RX_ADDRESS_P5 0x0F
#define REGISTER_TX_ADDRESS 0x10
#define REGISTER_RX_PAYLOAD_WIDTH_P0 0x11
#define REGISTER_RX_PAYLOAD_WIDTH_P1 0x12
#define REGISTER_RX_PAYLOAD_WIDTH_P2 0x13
#define REGISTER_RX_PAYLOAD_WIDTH_P3 0x14
#define REGISTER_RX_PAYLOAD_WIDTH_P4 0x15
#define REGISTER_RX_PAYLOAD_WIDTH_P5 0x16
#define REGISTER_FIFO_STATUS 0x17

static byte __code powerTable[] = {
    0x01,  // -18db
    0x03,  // -12db
    0x05,  // -6db
    0x07   // 0db
};

#define PAIR_CHANNEL 33
#define PAIR_POWER 0
static byte __code pairAddress[] = {
    'R', 'C', 'M', 'I', 'N'};

#define WORK_CHANNEL 66
#define WORK_POWER 3
static byte address[5];

static byte SPIReadWrite(byte data)
{
    byte i = 0;
    for (i = 0; i < 8; i++) {
        MOSI = (data & 0x80) == 0 ? 0 : 1;
        SCK = 1;
        data <<= 1;
        data |= MISO;
        SCK = 0;
    }
    return data;
}

static void Nrf24l01RegisterWrite(byte address, byte data)
{
    CSN = 0;
    SPIReadWrite(COMMAND_WRITE_REGISTER | address);
    SPIReadWrite(data);
    CSN = 1;
}

static byte Nrf24l01RegisterRead(byte address)
{
    byte data;
    CSN = 0;
    SPIReadWrite(COMMAND_READ_REGISTER | address);
    data = SPIReadWrite(0);
    CSN = 1;
    return data;
}

void Nrf24l01BufferWrite(byte *buffer, byte length)
{
    byte i = 0;
    CSN = 0;
    SPIReadWrite(COMMAND_WRITE_TX_PAYLOAD);
    for (i = 0; i < length; i++) {
        SPIReadWrite(buffer[i]);
    }
    CSN = 1;
}

int Nrf24l01BufferRead(byte *buffer, byte length)
{
    byte i = 0;
    if (IRQ == 0) {
        CSN = 0;
        SPIReadWrite(COMMAND_READ_RX_PAYLOAD);
        for (i = 0; i < length; i++) {
            buffer[i] = SPIReadWrite(0);
        }
        CSN = 1;

        CE = 0;
        Nrf24l01RegisterWrite(REGISTER_STATUS, 0x40);  //清除RX中断信号
        CE = 1;
        return length;
    } else {
        return -1;
    }
}

void Nrf24l01TxMode(void)
{
    CE = 0;
    Nrf24l01RegisterWrite(REGISTER_CONFIG, 0x0a);
    CE = 1;
}

void Nrf24l01RxMode(void)
{
    CE = 0;
    Nrf24l01RegisterWrite(REGISTER_CONFIG, 0x3b);  //CRC,8 bit,Power on,RX
    CE = 1;
}

static void Nrf24l01Channel(byte channel)
{
    CE = 0;
    Nrf24l01RegisterWrite(REGISTER_RF_CHANNEL, channel);
    CE = 1;
}

static void Nrf24l01Power(byte power)  //发射功率设置
{
    CE = 0;
    power %= sizeof(powerTable);
    Nrf24l01RegisterWrite(REGISTER_RF_SETUP, powerTable[power]);
    CE = 1;
}

static void Nrf24l01Size(byte size)
{
    CE = 0;
    Nrf24l01RegisterWrite(REGISTER_RX_PAYLOAD_WIDTH_P0, size);
    CE = 1;
}

void Nrf24l01TxAddress(byte *data)
{
    CSN = 0;
    SPIReadWrite(COMMAND_WRITE_REGISTER | REGISTER_TX_ADDRESS);
    SPIReadWrite(data[0]);
    SPIReadWrite(data[1]);
    SPIReadWrite(data[2]);
    SPIReadWrite(data[3]);
    SPIReadWrite(data[4]);
    CSN = 1;
}

void Nrf24l01RxAddress(byte *data)
{
    CSN = 0;
    SPIReadWrite(COMMAND_WRITE_REGISTER | REGISTER_RX_ADDRESS_P0);
    SPIReadWrite(data[0]);
    SPIReadWrite(data[1]);
    SPIReadWrite(data[2]);
    SPIReadWrite(data[3]);
    SPIReadWrite(data[4]);
    CSN = 1;
}

void Nrf24l01Init(void)
{
    CE = 0;
    SCK = 0;
    Nrf24l01RegisterWrite(REGISTER_ENABLE_AUTO_ACK, 0x00);     //禁止 自动应答
    Nrf24l01RegisterWrite(REGISTER_ENABLE_RX_ADDRESS, 0x01);   //允许 P0信道
    Nrf24l01RegisterWrite(REGISTER_SETUP_AUTO_RETRANS, 0x00);  //禁止 自动重发
    Nrf24l01TxMode();
    Nrf24l01Channel(WORK_CHANNEL);
    Nrf24l01Power(WORK_POWER);
    Nrf24l01Size(11);
    Nrf24l01TxAddress(address);
    Nrf24l01RxAddress(address);
}

void Nrf24l01PairMode(void)
{
    Nrf24l01Channel(PAIR_CHANNEL);
    Nrf24l01Power(PAIR_POWER);
    Nrf24l01TxAddress(pairAddress);
    Nrf24l01RxAddress(pairAddress);
}

void Nrf24l01WorkMode(void)
{
    Nrf24l01Channel(WORK_CHANNEL);
    Nrf24l01Power(WORK_POWER);
}

void Nrf24l01Send(void)
{
}

#ifdef __cplusplus
}
#endif
