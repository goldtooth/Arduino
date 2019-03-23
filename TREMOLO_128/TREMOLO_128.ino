
#include <SPI.h>         // Remember this line!
#include <DAC_MCP49xx.h>
#include <math.h>

// The Arduino pin used for the slave select / chip select
#define SS_PIN 10

// Set up the DAC. 
// First argument: DAC model (MCP4901, MCP4911, MCP4921)
// Second argument: SS pin (10 is preferred)
// (The third argument, the LDAC pin, can be left out if not used)
DAC_MCP49xx dac(DAC_MCP49xx::MCP4921, SS_PIN);

#define phototrans 9
#define relay 8

#define led1 0
#define led2 1
#define led3 6
#define led4 7

#define btn_onoff 3
#define btn_wave 2

#define ratepot A0
#define depthpot A1
#define shiftpot A2
//#define audiosignal A3

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

const PROGMEM uint16_t DACLookup_FullSine_6Bit[128] =
{
2048,2148,2248,2348,2447,2545,2642,2737,
2831,2923,3013,3100,3185,3267,3346,3423,
3495,3565,3630,3692,3750,3804,3853,3898,
3939,3975,4007,4034,4056,4073,4085,4093,
4095,4093,4085,4073,4056,4034,4007,3975,
3939,3898,3853,3804,3750,3692,3630,3565,
3495,3423,3346,3267,3185,3100,3013,2923,
2831,2737,2642,2545,2447,2348,2248,2148,
2048,1947,1847,1747,1648,1550,1453,1358,
1264,1172,1082,995,910,828,749,672,
600,530,465,403,345,291,242,197,
156,120,88,61,39,22,10,2,
0,2,10,22,39,61,88,120,
156,197,242,291,345,403,465,530,
600,672,749,828,910,995,1082,1172,
1264,1358,1453,1550,1648,1747,1847,1947,
};
const PROGMEM uint16_t DACLookup_FullTriangle_6Bit[128] =
{
64,128,192,256,320,384,448,512,
576,640,704,768,832,896,960,1024,
1088,1152,1216,1280,1344,1408,1472,1536,
1600,1664,1728,1792,1856,1920,1984,2048,
2111,2175,2239,2303,2367,2431,2495,2559,
2623,2687,2751,2815,2879,2943,3007,3071,
3135,3199,3263,3327,3391,3455,3519,3583,
3647,3711,3775,3839,3903,3967,4031,4095,
4031,3967,3903,3839,3775,3711,3647,3583,
3519,3455,3391,3327,3263,3199,3135,3071,
3007,2943,2879,2815,2751,2687,2623,2559,
2495,2431,2367,2303,2239,2175,2111,2048,
1984,1920,1856,1792,1728,1664,1600,1536,
1472,1408,1344,1280,1216,1152,1088,1024,
960,896,832,768,704,640,576,512,
448,384,320,256,192,128,64,0,

};

const PROGMEM uint16_t DACLookup_FullSquare_6Bit[128] =
{
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

const PROGMEM uint16_t DACLookup_Full24_6Bit[128] =
{
  1024, 2048, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 2048, 1024,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  1024, 2048, 4095, 4095, 4095, 4095, 4095, 4095,
  4095, 4095, 4095, 4095, 4095, 4095, 2048, 1024,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
};

void setup() {

    // Set the SPI frequency to 1 MHz (on 16 MHz Arduinos), to be safe.
  // DIV2 = 8 MHz works for me, though, even on a breadboard.
  // This is not strictly required, as there is a default setting.
  dac.setSPIDivider(SPI_CLOCK_DIV16);
  
  // Use "port writes", see the manual page. In short, if you use pin 10 for
  // SS (and pin 7 for LDAC, if used), this is much faster.
  // Also not strictly required (no setup() code is needed at all).
  dac.setPortWrite(true);
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
  attachInterrupt(digitalPinToInterrupt(btn_wave), CHANGEWAVE, LOW);
  pinMode(btn_onoff, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_onoff), ONANDOFF, LOW);

 
 //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
 

  sine();
 HITIT();
  //PORTB |= _BV(PB0);
}

void loop() {

  i++;
  if (i >= 128) {i = 0; 
  }
  if(i <= 64){
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
dac.output(h);


}

ISR(TIMER2_OVF_vect) {
 checkthings();
  TCNT2 = 130;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
}; 

void checkthings(){
  //read depth pot and scale
  deep = analogRead(depthpot);
  depth = map(deep, 0, 1023, 100, 0);
  depth   = (depth / 100);

  //read rate pot and add delay to make longer=
  rate = analogRead(ratepot);
  //rate = map(rate, 0, 1023, 0, 800); //last value here is the lowest frequency  (max 1023)
  rate = fscale( 0, 1023, 0, 1000, rate, 0);
  shift = analogRead(shiftpot);
  dong = map(shift, 0, 1023, 0, 100);
  wrong = map(shift, 0, 1023, 100, 0);
  dong  = (dong / 100);
  wrong  = (wrong / 100);

  Adel = rate*dong;
  Bdel = rate*wrong;
 
}

void ONANDOFF() {
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
HITIT();
}
}

void HITIT(){
     if (state == 0) {
      PORTB |= _BV(PB0); //8
      state = 1;
    }
    else {
      PORTB &= ~_BV(PB0); //8
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
PORTD |= _BV(PD0);
PORTD &= ~_BV(PD7);
breaker = 1;
}

void tri() {
PORTD |= _BV(PD1);
PORTD &= ~_BV(PD0);
breaker = 2;
}

void squarer() {
PORTD |= _BV(PD6);
PORTD &= ~_BV(PD1);
breaker = 3;
}

void dipper() {
PORTD |= _BV(PD7);
PORTD &= ~_BV(PD6);
breaker = 4;
}

float fscale( float originalMin, float originalMax, float newBegin, float
newEnd, float inputValue, float curve){

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output 
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println(); 
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){ 
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd; 
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine 
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {   
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange); 
  }

  return rangedValue;
} 
