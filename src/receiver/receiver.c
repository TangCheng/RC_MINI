#include <8052.h>

#include "datatype.h"
#include "drv/nrf24l01.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte rx[11];
static byte tx[11];
static byte hopping[5];
static byte address[5];

void PairWithController()
{
    byte t = 0;
    byte connecting = 0;
    Nrf24l01PairMode();
    Nrf24l01RxMode();
    while (Nrf24l01BufferRead(rx, 11) != 11) {
        ; // 读取接收数据
    }
    if (rx[0] == 0xa0) {
        hopping[0] = rx[1];
        hopping[1] = rx[2];
        hopping[2] = rx[3];
        hopping[3] = rx[4];
        hopping[4] = rx[5];
        address[0] = rx[6];
        address[1] = rx[7];
        address[2] = rx[8];
        address[3] = rx[9];
        address[4] = rx[10];
    }

    tx[0] = 'O', tx[1] = 'K';
    connecting = 1;
    while (connecting) {
        Nrf24l01TxMode();
        Nrf24l01BufferWrite(tx, 11);
        delay(1);

        Nrf24l01RxMode();
        // Nrf24l01Channel(hopping[0]);
        Nrf24l01TxAddress(address);
        Nrf24l01RxAddress(address);
        while (1) {
            delay(1);
            if (Nrf24l01BufferRead(rx, 11) == 11) {
                connecting = 0;
                break;
            }
            t++;
            if (t > 100) {
                t = 0;
                break;
            }
        }
    }
    //DATA_save();
    Nrf24l01WorkMode();
    Nrf24l01RxAddress(address);
    Nrf24l01TxAddress(address);
}

void main()
{
    LedUIDisplay("5678");
    RegisterTickProc(LedUITickProc);
    InitSysTick();
    StartTick();
    Nrf24l01Init();
    PairWithController();
    while (1) {
        P2_3 = 1;
        delay(250);
        P2_3 = 0;
        delay(250);
    }
}

void Timer0ISR() __interrupt(1)
{
    TickTimerProc();
}
