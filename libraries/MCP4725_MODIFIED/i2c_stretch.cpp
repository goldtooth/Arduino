
#include "i2c_stretch.h"

static void (*i2c_str_old_error_handler)(i2c_dev *dev);

void i2c_str_InitPort(i2c_dev *device, uint8_t divider)
{
  i2c_peripheral_disable(device);
  i2c_bus_reset(device);
  i2c_init(device);//initialize it
  i2c_config_gpios(device);//configure the gpios
  device->regs->CR2 = I2C_CR2_ITERREN | 36; //dma enabled, peripheral frequency is 36Mhz
  device->regs->CCR = I2C_CCR_FS | divider; //default 30
  device->regs->TRISE = 11;
}
void i2c_str_ReleasePort(i2c_dev *device)
{
  i2c_str_StopSending(device);
}

void i2c_str_StartSending(i2c_dev *device, uint8_t address)
{
  i2c_peripheral_enable(device);//enable the port
  i2c_start_condition(device);//set the start condition

  uint32_t sr1 = device->regs->SR1;
  uint32_t sr2 = device->regs->SR2;
  uint16_t wait = 0;
  while(!(sr1&I2C_SR1_SB))
  {
    if(wait++ > I2C_STRETCH_SB_TIMEOUT)
    {
      i2c_peripheral_disable(device);
      return;
    }
    sr1 = device->regs->SR1;
    sr2 = device->regs->SR2;
  }
  i2c_write(device, address<<1);//write the address of the device you want to contact (shifted 1 to the left)
}
void i2c_str_StopSending(i2c_dev *device)
{
  i2c_stop_condition(device);
  i2c_peripheral_disable(device);
}

bool i2c_str_IsSending(i2c_dev *device)
{
  return !(device->regs->SR1 & I2C_SR1_TXE);
}

bool i2c_str_IsError(i2c_dev *device)
{
  return device->regs->SR1 & (I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_AF | I2C_SR1_PECERR | I2C_SR1_TIMEOUT);
}

bool i2c_str_PortEnabled(i2c_dev *device)
{
  return device->regs->CR1 & I2C_CR1_PE;
}

void i2c_str_sendByte(i2c_dev *device, uint8_t data)
{
  uint32_t sr1 = device->regs->SR1;
  uint32_t sr2 = device->regs->SR2;
  if(sr1&(I2C_SR1_BTF | I2C_SR1_TXE | I2C_SR1_ADDR))
  {
    i2c_write(device, data);
  }
}

void i2c_str_sendBytes(i2c_dev *device, uint8_t data, uint8_t data2)
{
  uint32_t sr1 = device->regs->SR1;
  uint32_t sr2 = device->regs->SR2;
  if(sr1&(I2C_SR1_BTF | I2C_SR1_ADDR))
  {
    i2c_write(device, data);
    i2c_write(device, data2);
  }
}

