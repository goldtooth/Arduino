#include <Arduino.h>
#include "MCP45HVX1.h"

MCP45HVX1 digiPot(0x3C);
int analogPin = A3; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read
void setup()
{  

  digiPot.begin();  


             // Baseline Establish
}

void loop()
{
    val = (analogRead(analogPin)/4);  // read the input pin
delay(10);
  digiPot.writeWiper(val);
} 
