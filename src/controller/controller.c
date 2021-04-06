#include <8052.h>

#include "datatype.h"
#include "drv/nrf24l01.h"
#include "sys/tick.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

static byte tx_order[11];
static byte rx[11];
static byte hopping[5];
static byte address[5];

void PairWithReceiver()
{
    byte n = 0;
    byte cancel = 0;
    byte connecting = 1;
    byte string[4];
    Nrf24l01PairMode();

    tx_order[0] = 0xa0;
    tx_order[1] = hopping[0];
    tx_order[2] = hopping[1];
    tx_order[3] = hopping[2];
    tx_order[4] = hopping[3];
    tx_order[5] = hopping[4];
    tx_order[6] = address[0];
    tx_order[7] = address[1];
    tx_order[8] = address[2];
    tx_order[9] = address[3];
    tx_order[10] = address[4];

    cancel = 0;
    connecting = 1;
    while (!cancel & connecting)  //把对频信息发给接收机，若收到回复表明通信成功，
    {                             //收不到继续发送
        Nrf24l01TxMode();
        Nrf24l01BufferWrite(tx_order, 11);
        delay(1);
        Nrf24l01RxMode();

        while (1) {
            delay(1);
            if (Nrf24l01BufferRead(rx, 11) == 11) {
                if (rx[0] == 'O' && rx[1] == 'K') {
                    cancel = 0;
                    connecting = 0;
                }
            }

            n++;
            if (n > 50) {
                n = 0;
                break;
            }
        }
    }

    Nrf24l01WorkMode();
    Nrf24l01TxAddress(address);
    Nrf24l01RxAddress(address);
}

void main()
{
    LedUIDisplay("1234");
    RegisterTickProc(LedUITickProc);
    InitSysTick();
    StartTick();
    Nrf24l01Init();
    PairWithReceiver();
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
