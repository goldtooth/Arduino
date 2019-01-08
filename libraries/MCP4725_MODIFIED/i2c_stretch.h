/**
 * @file i2c_dma.h
 * @author Sven Michiels
 * @Library for driving the STM32 f103 i2c ports via DMA
 */

#ifndef _I2cStretch_H_
#define _I2cStretch_H_

#include <libmaple/i2c.h>
#include <wirish.h>

#define I2C_STRETCH_SB_TIMEOUT 10000 //how many times we will check the status registers to see if the device is starting

#define I2C_STRETCH_DIV_400KHZ 30
#define I2C_STRETCH_DIV_800KHZ 15
#define I2C_STRETCH_DIV_1200KHZ 10

struct I2cStretchSetting
{
  uint8_t slaveAddress;
};

static I2cStretchSetting I2cStretchSettings[2] = {{255},{255}};
static I2cStretchSetting* I2CStretch1 = I2cStretchSettings;
static I2cStretchSetting* I2CStretch2 = I2cStretchSettings + 1;

void i2c_str_InitPort(i2c_dev *dev, uint8_t divider = I2C_STRETCH_DIV_400KHZ);
void i2c_str_ReleasePort(i2c_dev *dev);
void i2c_str_StartSending(i2c_dev *dev, uint8_t address);
void i2c_str_StopSending(i2c_dev *dev);
bool i2c_str_IsSending(i2c_dev *dev);
bool i2c_str_IsError(i2c_dev *dev);
bool i2c_str_PortEnabled(i2c_dev *dev);
void i2c_str_sendByte(i2c_dev *dev, uint8_t data);
void i2c_str_sendBytes(i2c_dev *dev, uint8_t data, uint8_t data2);

static void i2c_str_errIrqHandler(i2c_dev *device);

#endif
