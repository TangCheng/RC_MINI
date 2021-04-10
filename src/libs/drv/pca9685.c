#include "drv/pca9685.h"

#include "bus/i2c.h"
#include "datatype.h"
#include "ui/led_ui.h"
#include "utils/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

// REGISTER ADDRESSES
#define PCA9685_MODE1 0x00      /**< Mode Register 1 */
#define PCA9685_MODE2 0x01      /**< Mode Register 2 */
#define PCA9685_SUBADR1 0x02    /**< I2C-bus subaddress 1 */
#define PCA9685_SUBADR2 0x03    /**< I2C-bus subaddress 2 */
#define PCA9685_SUBADR3 0x04    /**< I2C-bus subaddress 3 */
#define PCA9685_ALLCALLADR 0x05 /**< LED All Call I2C-bus address */
#define PCA9685_LED0_ON_L 0x06  /**< LED0 on tick, low byte*/
#define PCA9685_LED0_ON_H 0x07  /**< LED0 on tick, high byte*/
#define PCA9685_LED0_OFF_L 0x08 /**< LED0 off tick, low byte */
#define PCA9685_LED0_OFF_H 0x09 /**< LED0 off tick, high byte */
// etc all 16:  LED15_OFF_H 0x45
#define PCA9685_ALLLED_ON_L 0xFA  /**< load all the LEDn_ON registers, low */
#define PCA9685_ALLLED_ON_H 0xFB  /**< load all the LEDn_ON registers, high */
#define PCA9685_ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, low */
#define PCA9685_ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers,high */
#define PCA9685_PRESCALE 0xFE     /**< Prescaler for PWM output frequency */
#define PCA9685_TESTMODE 0xFF     /**< defines the test mode to be entered */

// MODE1 bits
#define MODE1_ALLCAL 0x01  /**< respond to LED All Call I2C-bus address */
#define MODE1_SUB3 0x02    /**< respond to I2C-bus subaddress 3 */
#define MODE1_SUB2 0x04    /**< respond to I2C-bus subaddress 2 */
#define MODE1_SUB1 0x08    /**< respond to I2C-bus subaddress 1 */
#define MODE1_SLEEP 0x10   /**< Low power mode. Oscillator off */
#define MODE1_AI 0x20      /**< Auto-Increment enabled */
#define MODE1_EXTCLK 0x40  /**< Use EXTCLK pin clock */
#define MODE1_RESTART 0x80 /**< Restart enabled */
// MODE2 bits
#define MODE2_OUTNE_0 0x01 /**< Active LOW output enable input */
#define MODE2_OUTNE_1 0x02 /**< Active LOW output enable input - high impedience */
#define MODE2_OUTDRV 0x04  /**< totem pole structure vs open-drain */
#define MODE2_OCH 0x08     /**< Outputs change on ACK vs STOP */
#define MODE2_INVRT 0x10   /**< Output logic state inverted */

#define PCA9685_I2C_ADDRESS 0x80       /**< Default PCA9685 I2C Slave Address */
#define FREQUENCY_OSCILLATOR 25000000L /**< Int. osc. frequency in datasheet */

#define PCA9685_PRESCALE_MIN 3   /**< minimum prescale value */
#define PCA9685_PRESCALE_MAX 255 /**< maximum prescale value */

#define PULSE_STEPS 4096
#define MAX_PINS 16

static bool Pca9685ReadRegister(byte addr, byte *data)
{
    bool ack = false;
    I2cStart();  // 启动总线
    ack = I2cSendByte(PCA9685_I2C_ADDRESS);  // 发送器件地址
    if (ack == false) {
        return ack;
    }
    ack = I2cSendByte(addr);  // 发送寄存器地址
    if (ack == false) {
        return ack;
    }
    I2cStop();  // 结束总线

    I2cStart();                                     // 启动总线
    ack = I2cSendByte(PCA9685_I2C_ADDRESS | 0x01);  // 发送器件地址
    if (ack == false) {
        return ack;
    }
    *data = I2cReceiveByte();  // 读取数据0

    I2cAcknowledge(false);  // 发送非应答信号
    I2cStop();             // 结束总线
    return ack;
}

static bool Pca9685WriteRegister(byte addr, byte data)
{
    bool ack = false;
    I2cStart();                              // 启动总线
    ack = I2cSendByte(PCA9685_I2C_ADDRESS);  // 发送器件地址
    if (ack == false) {
        return ack;
    }
    ack = I2cSendByte(addr);  // 发送寄存器地址
    if (ack == false) {
        return ack;
    }
    ack = I2cSendByte(data);
    if (ack == false) {
        return ack;
    }
    I2cStop();  // 结束总线
    return ack;
}

void Pca9685Init()
{
    Pca9685WriteRegister(PCA9685_MODE1, 0x00);
    DelayMs(5);
}

void Pca9685SetPwmFrequency(byte frequency)
{
    if (frequency < 1) {
        frequency = 1;
    }
    byte prescale = FREQUENCY_OSCILLATOR / PULSE_STEPS / frequency - 1;
    if (prescale <= PCA9685_PRESCALE_MIN) {
        prescale = PCA9685_PRESCALE_MIN;
    }

    byte pca9685Mode = 0;
    LedUIDisplay("       A");
    if (Pca9685ReadRegister(PCA9685_MODE1, &pca9685Mode) == false) {
        return;
    }
    LedUIDisplay("       B");
    if (Pca9685WriteRegister(PCA9685_MODE1, (pca9685Mode & ~MODE1_RESTART) | MODE1_SLEEP) == false) {
        return;
    }
    LedUIDisplay("       C");
    if (Pca9685WriteRegister(PCA9685_PRESCALE, prescale) == false) {
        return;
    }
    LedUIDisplay("       D");
    if (Pca9685WriteRegister(PCA9685_MODE1, pca9685Mode) == false) {
        return;
    }
    LedUIDisplay("       E");
    DelayMs(5);
    Pca9685WriteRegister(PCA9685_MODE1, pca9685Mode | MODE1_RESTART);
}

void Pca9685Reset()
{
    byte pca9685Mode = 0;
    if (Pca9685ReadRegister(PCA9685_MODE1, &pca9685Mode) == false) {
        return;
    }
    if (pca9685Mode & MODE1_RESTART) {
        Pca9685WriteRegister(PCA9685_MODE1, pca9685Mode | MODE1_SLEEP);
        DelayMs(1);
    }
    Pca9685WriteRegister(PCA9685_MODE1, MODE1_RESTART);
    DelayMs(10);
}

static void Pca9685SetPwm(byte registerOffset, word on, word off)
{
    registerOffset <<= 2;
    Pca9685WriteRegister(PCA9685_LED0_ON_L + registerOffset, (byte)(on & 0x00FF));
    Pca9685WriteRegister(PCA9685_LED0_ON_H + registerOffset, (byte)(on >> 8));
    Pca9685WriteRegister(PCA9685_LED0_OFF_L + registerOffset, (byte)(off & 0x00FF));
    Pca9685WriteRegister(PCA9685_LED0_OFF_H + registerOffset, (byte)(off >> 8));
}

void Pca9685SetPin(byte pinNum, word turnOnSteps)
{
    if (pinNum > MAX_PINS) {
        pinNum = MAX_PINS;
    }
    if (turnOnSteps >= PULSE_STEPS) {
        turnOnSteps = PULSE_STEPS - 1;
    }
    if (turnOnSteps == PULSE_STEPS - 1) {
        Pca9685SetPwm(pinNum - 1, PULSE_STEPS, 0);
    } else if (turnOnSteps == 0) {
        Pca9685SetPwm(pinNum - 1, 0, PULSE_STEPS);
    } else {
        Pca9685SetPwm(pinNum - 1, 0, turnOnSteps);
    }
}

#ifdef __cplusplus
}
#endif
