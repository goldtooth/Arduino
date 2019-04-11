

#include <Wire.h>
#include "mcp4728.h"
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>


HardwareTimer timer(2);
#define TIMER_RATE 10000



////do not define pins within global!!!!!

long debouncing_time = 150; //Debouncing Time in Milliseconds
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
int POT1;
int POT2;
int POT3;
int POT4;
int POT5;
int hate;
int AUDIN;

void setup() {
disableDebugPorts();
dac.begin();  // initialize i2c interface
dac.vdd(3300); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
dac.setVref(0,0,0,0);
//dac.setVref(1,1,1,1);


  pinMode(PA8, OUTPUT);
  pinMode(PA9, OUTPUT);
  pinMode(PA10, OUTPUT);
  pinMode(PA11, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB5, OUTPUT);


  pinMode(PA12, OUTPUT); //relay
  pinMode(PA15, OUTPUT); //relay


  pinMode(PA0, INPUT_ANALOG);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  pinMode(PA4, INPUT);
  pinMode(PA5, INPUT);


      // disable all interrupts
  // Setup the second button with an internal pull-up :
  pinMode(PB13, INPUT_PULLUP);
  attachInterrupt(PB13, CHANGEPRESET, FALLING); // or rising / or change

  
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, ONANDOFF, FALLING); // or rising / or change


//setupstm timer
timer.pause();
timer.setPeriod(TIMER_RATE); // in microseconds
  // Set up an interrupt on channel 1
timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
timer.attachCompare1Interrupt(handler);
timer.refresh();
timer.resume();



digitalWrite(PA8,LOW);
digitalWrite(PA9,LOW);
digitalWrite(PA10,LOW);
digitalWrite(PA11,LOW);
digitalWrite(PB4,LOW);
digitalWrite(PB5,LOW);
  one();
}

void loop() {



dac.analogWrite(POT1,POT2,POT3,AUDIN+POT4);;
}

void handler(void) {
 checkthings();
}; 





void checkthings(){
 //read depth pot and scale
  POT1 = analogRead(PA1);
    POT2 = analogRead(PA2);
      POT3 = analogRead(PA3);
        POT4 = analogRead(PA4);
          POT5 = analogRead(PA5);
          AUDIN = analogRead(PA0);
}




void ONANDOFF() {
if ((long)(micros() - last_micros) >= debouncing_time * 1500) {
TOGGLE_SS_RELAY();
TOGGLE_HW_RELAY();
last_micros = micros();

}

}

void TOGGLE_HW_RELAY(){
if (state == 0) {
  digitalWrite(PA15, HIGH);
     // PORTA |= _BV(PA15); //8
      state = 1;
    }
    else {
      digitalWrite(PA15, LOW);
     // PORTA &= ~_BV(PA15); //8
      state = 0;
    }
    TOGGLE_SS_RELAY();
 } 


void TOGGLE_SS_RELAY(){
if (hate == 0) {
    
    digitalWrite(PA12,HIGH);
      hate = 1;
    }
    else {
    digitalWrite(PA12,LOW);
     hate = 0;
    }
  }
  

void CHANGEPRESET() {
  if ((long)(micros() - last_micros2) >= debouncing_time * 1500) {
   
   
    led++;
    if (led >= 7) {
      led = 1;
    }
    last_micros2 = micros();
  }
  switch (led) {
    case 1:
      one();
      break;

    case 2:
      two();
      break;

    case 3:
      three();
      break;

    case 4:     
      four();
      break;

    case 5:
      five();
      break;

    case 6:     
      six();
      break;
  }
}

void one() {
digitalWrite(PA8, HIGH);
digitalWrite(PB5, LOW);  
breaker = 1;
}

void two() {
digitalWrite(PA9, HIGH);
digitalWrite(PA8, LOW);  
breaker = 2;
}

void three() {
digitalWrite(PA10, HIGH);
digitalWrite(PA9, LOW);  
breaker = 3;
}

void four() {
digitalWrite(PA11, HIGH);
digitalWrite(PA10, LOW);  
breaker = 4;
}

void five() {
digitalWrite(PB4, HIGH);
digitalWrite(PA11, LOW);  
breaker = 5;
}

void six() {
digitalWrite(PB5, HIGH);
digitalWrite(PB4, LOW);  
breaker = 6;
}
