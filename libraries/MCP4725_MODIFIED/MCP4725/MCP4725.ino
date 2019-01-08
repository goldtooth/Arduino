#include "i2c_stretch.h"

#define DACDevice I2C1
#define DACAddress 0x60
uint16_t sineLookup[20] = {2048,2680,3251,3704,3995,4095,3995,3704,3251,2680,2048,1415,844,391,100,0,100,391,844,1415};
byte sinePos = 0;

bool toggle = 0;

void setup() 
{
  pinMode(33, OUTPUT);
  startCommunication();
  initTimer();
}

void loop() 
{
  if(i2c_str_IsError(DACDevice) | !i2c_str_PortEnabled(DACDevice))
  {
    digitalWrite(33, toggle = !toggle);
    startCommunication();
  }
  delay(100);
}

void nextSample()
{
  i2c_str_sendBytes(DACDevice, sineLookup[sinePos]>>8, sineLookup[sinePos++] & 255);
  sinePos%=20;
}

void startCommunication()
{
  i2c_str_InitPort(DACDevice, I2C_STRETCH_DIV_1200KHZ);
  i2c_str_StartSending(DACDevice, DACAddress);
}

void initTimer()
{
  nvic_irq_set_priority(NVIC_TIMER2, 0);
  Timer2.setChannel1Mode(TIMER_OUTPUTCOMPARE);
  Timer2.setPrescaleFactor(9);
  Timer2.setOverflow(133); //should be about 60khz
  Timer2.setCompare1(1);
  Timer2.attachCompare1Interrupt(nextSample);
}

