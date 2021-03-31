#include <8052.h>

#define LSA P0_5
#define LSB P0_6
#define LSC P0_7

void EXint_Init(void);
void Delayms(unsigned int);
void Down2Up(int);
void Up2Down(int);
void EXINT0() __interrupt 0;
void EXINT1() __interrupt 2;

void main()
{
    EXint_Init();
    LSA = 0;
    LSB = 0;
    LSC = 0;
    while (1) {
        P1 = 0xff;
        Delayms(250);
        P1 = 0x00;
        Delayms(250);
    }
}

void EXint_Init()
{
    IT0 = 1;
    IT1 = 0;
    //	IPH=0x40;
    PX1 = 1;
    EA = 1;
    EX0 = 1;
    EX1 = 1;
}

void Delayms(unsigned int xms)
{
    unsigned int i, j;
    for (i = xms; i > 0; i--) {
        for (j = 110; j > 0; j--)
            ;
    }
}

void EXINT0() __interrupt 0
{
    Down2Up(3);
}

void EXINT1() __interrupt 2
{
    Up2Down(3);
}

void Down2Up(int x)
{
    int i, j;
    unsigned char sel = 0xfe;
    for (i = 0; i < x; i++) {
        for (j = 0; j < 8; j++) {
            P0 = sel;
            Delayms(250);
            sel = sel << 1;
        }
        sel = 0xfe;
    }
}

void Up2Down(int x)
{
    int i, j;
    unsigned char sel = 0x7f;
    for (i = 0; i < x; i++) {
        for (j = 0; j < 8; j++) {
            P0 = sel;
            Delayms(250);
            sel = sel >> 1;
        }
        sel = 0x7f;
    }
}
