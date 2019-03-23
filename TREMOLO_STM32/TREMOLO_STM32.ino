

#include <Wire.h>
#include "mcp4728.h"
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>


HardwareTimer timer(2);
#define TIMER_RATE 10000



////do not define pins within global!!!!!
const int phototrans = 30;
const int relay = 32;



const int btn_onoff = 8;

#define OTHERPOT 3

long debouncing_time = 100; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
int poop = 1;
int led = 1;
int i;
int b;
uint16_t h;
int breaker = 1;
int rate;
int state = 0;
int val;
int actdelay;
float dong;
float wrong;
float Adel;
float Bdel;
float deep;
float depth;
float shift;
int timer1_counter;

// using a power of 2 for the filterWeight will allow the compiler to optimise the calculation 
const int filterWeight = 16; // higher numbers = heavier filtering 
const int numReadings = 20 ;
int average;


const PROGMEM uint16_t DACLookup_FullSine_6Bit[64] =
{
  2048, 2248, 2447, 2642, 2831, 3013, 3185, 3346,
  3495, 3630, 3750, 3853, 3939, 4007, 4056, 4085,
  4095, 4085, 4056, 4007, 3939, 3853, 3750, 3630,
  3495, 3346, 3185, 3013, 2831, 2642, 2447, 2248,
  2048, 1847, 1648, 1453, 1264, 1082,  910,  749,
   600,  465,  345,  242,  156,   88,   39,   10,
     0,   10,   39,   88,  156,  242,  345,  465,
   600,  749,  910, 1082, 1264, 1453, 1648, 1847,
   
};
const PROGMEM uint16_t DACLookup_FullTriangle_6Bit[64] =
{
  0,   10,   39,   88,  156,  242,  345,  465,
   600,  749,  910, 1082, 1264, 1453, 1648, 1847,
  2048, 2248, 2447, 2642, 2831, 3013, 3185, 3346,
  3495, 3630, 3750, 3853, 3939, 4007, 4056, 4085,
  4095, 4085, 4056, 4007, 3939, 3853, 3750, 3630,
  3495, 3346, 3185, 3013, 2831, 2642, 2447, 2248,
  2048, 1847, 1648, 1453, 1264, 1082,  910,  749,
   600,  465,  345,  242,  156,   88,   39,   10,

};

const PROGMEM uint16_t DACLookup_FullSquare_6Bit[64] =
{
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

const PROGMEM uint16_t DACLookup_Full24_6Bit[64] =
{
0, 0, 0, 0, 0, 0, 0, 0,
1082, 2048, 4095, 4095, 4095, 4095, 2048, 1082,
0, 0, 0, 0, 0, 0, 0, 0,
1082, 2048, 4095, 4095, 4095, 4095, 4095, 4095,
4095,4095,4095,4095,4095,4095,4095,4095,
4095,4095,4095,4095,4095,4095,4095,4095,
4095,4095,4095,4095,4095,4095,4095,4095,
4095,4095,4095,4095,4095,4095,4095,4095
};

void setup() {

dac.begin();  // initialize i2c interface
dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
dac.setVref(0,0,0,0);

  pinMode(PA6, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB1, OUTPUT);

  pinMode(relay, OUTPUT); //relay



  pinMode(PA0, INPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);

      // disable all interrupts
  // Setup the second button with an internal pull-up :
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, CHANGEWAVE, FALLING); // or rising / or change

  
  pinMode(btn_onoff, INPUT_PULLUP);
  attachInterrupt(btn_onoff, ONANDOFF, FALLING); // or rising / or change



//setupstm timer
 timer.pause();
timer.setPeriod(TIMER_RATE); // in microseconds
  // Set up an interrupt on channel 1
timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
timer.attachCompare1Interrupt(handler);
timer.refresh();
timer.resume();

 sine();
 HITIT();
}

void loop() {
  

  i++;
  if (i >= 64) {i = 0; 
  }
  if(i <= 32){
    actdelay = Adel;
    }
  else{
    actdelay = Bdel;
    }

  if (breaker == 1) {
    h = ((pgm_read_word(&(DACLookup_FullSine_6Bit[i])) * depth));
  }
  else if (breaker == 2) {
    h = ((pgm_read_word(&(DACLookup_FullTriangle_6Bit[i])) * depth));
  }
  else if (breaker == 3) {
    h = ((pgm_read_word(&(DACLookup_FullSquare_6Bit[i])) * depth));
  }
  else if (breaker == 4) {
    h = ((pgm_read_word(&(DACLookup_Full24_6Bit[i])) * depth));
  }
delayMicroseconds(rate);
dac.voutWrite(shift, deep, deep, rate);
  

}

void handler(void) {
 checkthings();
}; 

void checkthings(){
 //read depth pot and scale
  deep = analogRead(PA1);
  depth = map(deep, 0, 4095, 100, 0);
  depth   = (depth / 100);

  //read rate pot and add delay to make longer=
  rate = analogRead(PA2);
  //rate = map(rate, 0, 1023, 0, 800); //last value here is the lowest frequency  (max 1023)
  //rate = map(rate, 0, 4095, 10, 1000);
  
  shift = analogRead(PA3);
  dong = map(shift, 0, 4095, 0, 100);
  wrong = map(shift, 0, 4095, 100, 0);
  dong  = (dong / 100);
  wrong  = (wrong / 100);

  Adel = rate*dong;
  Bdel = rate*wrong;

 
for (int k = 0; k < numReadings; k++) { average = average + (rate - average) / filterWeight; }

  

}

void ONANDOFF() {
  if ((long)(micros() - last_micros) >= debouncing_time * 10) {
HITIT();
}
}

void HITIT(){
   poop++;
    if (poop >= 4) {
     poop = 1;
    }
    switch (led) {
    case 1:
      relay == LOW;
      phototrans ==LOW;
      break;
    case 2:
      relay == HIGH;
      phototrans ==LOW;
      break;
          case 3:
      relay == LOW;
      phototrans ==HIGH;
      break;
         
    }


    
    last_micros = micros();
//PORTB |= _BV(PB0);
//PORTB &= ~_BV(PB0);
 } 
  
void CHANGEWAVE() {
  if ((long)(micros() - last_micros2) >= debouncing_time * 1000) {
   
    ONANDOFF();
    led++;
    if (led >= 5) {
      led = 1;
    }
    last_micros2 = micros();
  }
  switch (led) {
    case 1:
      sine();
      break;

    case 2:
      tri();
      break;

    case 3:
      squarer();
      break;

    case 4:
      
     
      dipper();
      break;
  }
}

void sine() {
digitalWrite(PA6, HIGH);
digitalWrite(PB1, LOW);  
breaker = 1;
}

void tri() {
digitalWrite(PA7, HIGH);
digitalWrite(PA6, LOW);  
breaker = 2;
}

void squarer() {
digitalWrite(PB0, HIGH);
digitalWrite(PA7, LOW);  
breaker = 3;
}

void dipper() {
digitalWrite(PB1, HIGH);
digitalWrite(PB0, LOW);  
breaker = 4;
}
