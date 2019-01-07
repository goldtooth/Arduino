
#include <MCP4922.h>
#include <SPI.h>

MCP4922 dac1(51,52,10,5);    // (MOSI,SCK,CS,LDAC) define Connections for MEGA_board, 
//MCP4922 DAC(11,13,10,5);    // (MOSI,SCK,CS,LDAC) define Connections for UNO_board, 
MCP4922 dac2(51,52,10,5);  
HardwareTimer timer(2);
#define TIMER_RATE 10000

#define phototrans 30
#define relay PC13

#define led1 38
#define led2 39
#define led3 40
#define led4 41

#define btn_onoff 3
#define btn_wave 28

#define ratepot 0
#define depthpot 1
#define shiftpot 2
//#define audiosignal 3

long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
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

    // Set the SPI frequency to 1 MHz (on 16 MHz Arduinos), to be safe.
  // DIV2 = 8 MHz works for me, though, even on a breadboard.
  // This is not strictly required, as there is a default setting.
  //dac1.setSPIDivider(SPI_CLOCK_DIV16);
  
  // Use "port writes", see the manual page. In short, if you use pin 10 for
  // SS (and pin 7 for LDAC, if used), this is much faster.
  // Also not strictly required (no setup() code is needed at all).
  //dac1.setPortWrite(true);
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(relay, OUTPUT); //relay



  pinMode(ratepot, INPUT);
  pinMode(depthpot, INPUT);
  pinMode(shiftpot, INPUT);
      // disable all interrupts
  // Setup the second button with an internal pull-up :
  pinMode(btn_wave, INPUT_PULLUP);
  attachInterrupt(btn_wave, CHANGEWAVE, FALLING); // or rising / or change
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
 
 //Setup Timer2 to fire every 1ms
 // TCCR2B = 0x00;        //Disbale Timer2 while we set it up
 // TCNT2  = 130;         //Reset Timer Count to 130 out of 255
 // TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  //TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
 // TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
 // TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
 

  sine();
 HITIT();
  //PORTB |= _BV(PB0);
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
//uuuuuuu uuuuuuuuuuuuu
// maybe shift change that I is faster / slower

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
delayMicroseconds(actdelay);
dac1.Set(h);
dac2.Set(h);

}

void handler(void) {
 checkthings();
}; 

void checkthings(){
  //read depth pot and scale
  deep = analogRead(depthpot);
  depth = map(deep, 0, 1023, 100, 0);
  depth   = (depth / 100);

  //read rate pot and add delay to make longer=
  rate = analogRead(ratepot);
  rate = map(rate, 0, 1023, 0, 800); //last value here is the lowest frequency  (max 1023)
  
  shift = analogRead(shiftpot);
  dong = map(shift, 0, 1023, 0, 100);
  wrong = map(shift, 0, 1023, 100, 0);
  dong  = (dong / 100);
  wrong  = (wrong / 100);

  Adel = rate*dong;
  Bdel = rate*wrong;
 
}

void ONANDOFF() {
  if ((long)(micros() - last_micros) >= debouncing_time * 10) {
HITIT();
}
}

void HITIT(){
     if (state == 0) {
      relay == HIGH; 
      state = 1;
    }
    else {
      relay == LOW; 
      state = 0;
    }
    last_micros = micros();
//PORTB |= _BV(PB0);
//PORTB &= ~_BV(PB0);
 } 
  
void CHANGEWAVE() {
  if ((long)(micros() - last_micros2) >= debouncing_time * 1000) {
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
digitalWrite(led1, HIGH);
digitalWrite(led4, LOW);  
breaker = 1;
}

void tri() {
digitalWrite(led2, HIGH);
digitalWrite(led1, LOW);  
breaker = 2;
}

void squarer() {
digitalWrite(led3, HIGH);
digitalWrite(led2, LOW);  
breaker = 3;
}

void dipper() {
digitalWrite(led4, HIGH);
digitalWrite(led3, LOW);  
breaker = 4;
}
