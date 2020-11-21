#include <SPI.h>         // Remember this line!
#include <DAC_MCP49xx.h>
#include <math.h>
#include <EEPROM.h>

// The Arduino pin used for the slave select / chip select
#define SS_PIN 10

// Set up the DAC. 
// First argument: DAC model (MCP4901, MCP4911, MCP4921)
// Second argument: SS pin (10 is preferred)
// (The third argument, the LDAC pin, can be left out if not used)
DAC_MCP49xx dac(DAC_MCP49xx::MCP4921, SS_PIN);

#define phototrans 9
#define relay 8
int delaytime = 20000;
#define led1 0
#define led2 1
#define led3 6
#define led4 7
#define STATE_NORMAL 0 //static const int  STATE_NORMAL = 0; // no button activity
#define STATE_SHORT 1 //static const int  STATE_SHORT  = 1; // short button press
#define STATE_LONG 2 //static const int  STATE_LONG  = 2; // long button press
volatile int  resultButton = 0; // global value set by checkButton()
#define btn_onoff 3
#define btn_wave 2
 
#define ratepot A0
#define depthpot A1
#define shiftpot A2
#define audioin A7
//#define audiosignal A3

long debouncing_time = 250; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
volatile unsigned long last_micros3; 
bool peakos = false;
int iii;
int led;
int i;
int b; 
int enact = 1;
int wavetablelength = 512;
int relay_damper_ms = 10;
uint16_t h;
float prevData = 0;
float newData =0;
float peak;
int breaker = 1;
int rate;
int state = 0;
int hate = 0;
float val;
float low;
float SHORT1;
float Threshold;
float RAWIN;
float adjust;
float attack;
float input_bias = 0.68; // 
float release;
bool comp_status_on ;
int timer1_counter;
const int SHORT1_AVG_SAMPLE = 500;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int SHORT1_AVG_SAMP[SHORT1_AVG_SAMPLE] = {0}; //value of sample
int SHORT1_AVG_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int SHORT1_AVG_TOTAL = 0;
//----------------------------
//gate settings
int gate_on_response = 50; // difference between SHORT1 & 2 needed for gate to turn on 
int SHORT1_SMOOTHING = 10; // SHORT1 Smoothing
int ROLLOFF = 10;  //SPEED THAT GATE SHUTS OFF (higher number = faster.)
float ratio = 0.05;
float working_ratio = 1;

float slope = 0.01;// roll off from peaks
/*
2:1 = 0.5
4:1 = 0.25
20:1 = 0.05

*/

bool rms = false;


void setup() {

    // Set the SPI frequency to 1 MHz (on 16 MHz Arduinos), to be safe.
  // DIV2 = 8 MHz works for me, though, even on a breadboard.
  // This is not strictly required, as there is a default setting.
  dac.setSPIDivider(SPI_CLOCK_DIV2);
  
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
  pinMode(phototrans, OUTPUT); //relay



  pinMode(ratepot, INPUT);
  pinMode(depthpot, INPUT);
  pinMode(shiftpot, INPUT);
    pinMode(audioin, INPUT);
      // disable all interrupts
  // Setup the second button with an internal pull-up :
  pinMode(btn_wave, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_wave), checkButton, CHANGE); //FALLING? OR RISING? as a way to not have theinterupt triggering when depressed for a long time
  pinMode(btn_onoff, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_onoff), ONANDOFF, CHANGE);

 
 //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 230;         //Reset Timer Count to 130 out of 255 - counter is 255-count (ie 205 -= 50)
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
  TCCR2B = 0x05;        //Timer2 Control Reg B: Timer Prescaler set to 128
 
digitalWrite(phototrans, LOW);
  //sine()d
led = EEPROM.read(1);
  //PORTB |= _BV(PB0);
switch (led) {
    case 1:
      ratio_2_1();
      break;

    case 2:
      ratio_5_1();
      break;

    case 3:
      ratio_8_1();
      break;

    case 4:
 ratio_20_1();
      break;
  }
}


extern "C" {
ISR(TIMER2_OVF_vect) {
 checkthings();
  TCNT2 = 230;           //Reset Timer to 130 out of 255
  TIFR2 = 0x00;          //Timer2 INT Flag Reg: Clear Timer Overflow Flag
}; 
}

void loop() {
//---------------------------------------------*Dynamics-------------------------------------------
 int longButton=0;
 

  switch (resultButton) {
  
  case STATE_NORMAL: {
  break;
  }

  case STATE_SHORT: {
  CHANGEWAVE();
  resultButton=STATE_NORMAL;
  break;
  }

  case STATE_LONG: {
  PEAK_RMS();
  longButton++;
  resultButton=STATE_NORMAL;
  break;
  }
  }



//RAWIN = RAWIN - input_bias;

//RAWIN = map(RAWIN, 0, 1024, 0, 1023);







if((RAWIN>Threshold)) // if short is over the threashold
{
if(!comp_status_on){
  comp_status_on = true; 
PORTD |= _BV(PD0);
} //if status is off, turn it on, turn on LED

working_ratio = working_ratio - attack;
if(working_ratio <= ratio){working_ratio=ratio;}

}else{ // should be else the signal over the Threshold 

if(comp_status_on){working_ratio = working_ratio+release;} 

if (working_ratio >= 1.00){
working_ratio =1.00; 
comp_status_on = false; 
PORTD &= ~_BV(PD0);
}
 }
adjust = RAWIN-((Threshold+((RAWIN-Threshold)*working_ratio))); //what is in the brackets is the compressed signal, total equation is the difference.






h  = 0 + (adjust);//adjust*20;//from 1023 to 4096

if (h >4095){h=4095;}
if (h <1){h=1;}
//4095 SEEMS TO BE SILENT, 0 FULL VOLUME
//adjust back for RAWIN's input adjustments
dac.output(h);



}


void checkthings(){
  //read depth pot and scale
  attack = analogRead(depthpot);
  //attack = map(attack , 0, 1023, 0, 2000);
  attack = attack/10000;

  //read rate pot and add delay to make longer=
  release = analogRead(ratepot);
 //release = map(release , 0, 1023, 0, 2000);
  release = release/1000;
  //rate = fscale( 0, 1023, 100, 2000, rate, -4);
  
  Threshold = analogRead(shiftpot);
  Threshold  = map(Threshold , 0, 1023, 3000, 0);
   //Threshold = Threshold*4;
RAWIN = analogRead(audioin);
RAWIN = (RAWIN - ((1023/5)*input_bias))*4;// roughly minus the milllivolts
if(rms){ RAWIN  =  RAWIN  * 0.7071;}   //RMS                                  
if(RAWIN<=0){RAWIN = 0;}


  //rate = fscale( 0, 1023, 100, 2000, rate, -4);  
  //---------------------------------------------*AVERAGING*------------------------------------------------------------------*/
SHORT1_AVG_TOTAL -= SHORT1_AVG_SAMP[SHORT1_AVG_INDEX];   //Subtracts the last sample
SHORT1_AVG_SAMP[SHORT1_AVG_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
SHORT1_AVG_TOTAL += SHORT1_AVG_SAMP[SHORT1_AVG_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (SHORT1_AVG_INDEX >= (SHORT1_SMOOTHING)){SHORT1_AVG_INDEX = 0;}
SHORT1 = SHORT1_AVG_TOTAL/SHORT1_SMOOTHING;

  
}





void ONANDOFF() {
if ((long)(micros() - last_micros) >= debouncing_time * 550) {
TOGGLE_HW_RELAY();
last_micros = micros();
}
}

void TOGGLE_HW_RELAY(){

digitalWrite(phototrans, HIGH);
delayMicroseconds(delaytime);
if (state == 0) {
      PORTB |= _BV(PB0); //8
      state = 1;
    }
    else {
      PORTB &= ~_BV(PB0); //8
      state = 0;
    }
delayMicroseconds(delaytime);
digitalWrite(phototrans, LOW);
   
 } 

void PEAK_RMS(){
  if(!rms){rms=true;}else{rms=false;}
  }


 
  
void CHANGEWAVE() {
  if ((long)(micros() - last_micros2) >= debouncing_time * 150) {
    led++;
    if (led >= 5) {
      led = 2;
    }
  EEPROM.write(1, led);
    last_micros2 = micros();
  }
   //tri();
 switch (led) {
    case 1:
      ratio_2_1();
      break;

    case 2:
      ratio_5_1();
      break;

    case 3:
      ratio_8_1();
      break;

    case 4:
      
      ratio_20_1();
      break;
  }
 
}

void  ratio_2_1() {
//PORTD |= _BV(PD0);
//PORTD &= ~_BV(PD7);


}

void  ratio_5_1() {
ratio = 0.2;
PORTD |= _BV(PD1);
PORTD &= ~_BV(PD7);
//PORTD &= ~_BV(PD0);
}

void  ratio_8_1() {
ratio = 0.125;
PORTD |= _BV(PD6);
PORTD &= ~_BV(PD1);

}

void  ratio_20_1() {
ratio = 0.005;
PORTD |= _BV(PD7);
PORTD &= ~_BV(PD6);

}


void checkButton() {
  /*
  * This function implements software debouncing for a two-state button.
  * It responds to a short press and a long press and identifies between
  * the two states. Your sketch can continue processing while the button
  * function is driven by pin changes.
  */

  const unsigned long LONG_DELTA = 300000;               // hold seconds for a long press
  const unsigned long DEBOUNCE_DELTA = 3000;        // debounce time
  static int lastButtonStatus = HIGH;                                   // HIGH indicates the button is NOT pressed
  int buttonStatus;                                                                    // button atate Pressed/LOW; Open/HIGH
  static unsigned long longTime = 0, shortTime = 0; // future times to determine is button has been poressed a short or long time
  boolean Released = true, Transition = false;                  // various button states
  boolean timeoutShort = false, timeoutLong = false;    // flags for the state of the presses

  buttonStatus = digitalRead(2);                // read the button state on the pin "BUTTON_PIN"
  timeoutShort = (micros() > shortTime);                          // calculate the current time states for the button presses
  timeoutLong = (micros() > longTime);


  if (buttonStatus != lastButtonStatus) {                          // reset the timeouts if the button state changed
      shortTime = micros() + DEBOUNCE_DELTA;
      longTime = micros() + LONG_DELTA;
  }

 
  Transition = (buttonStatus != lastButtonStatus);        // has the button changed state
  Released = (Transition && (buttonStatus == HIGH)); // for input pullup circuit

  lastButtonStatus = buttonStatus;                                     // save the button status

  if ( ! Transition) {                                                                //without a transition, there's no change in input
  // if there has not been a transition, don't change the previous result
       resultButton =  STATE_NORMAL | resultButton;
       return;
  }



  if (timeoutLong && Released) {                                      // long timeout has occurred and the button was just released
       resultButton = STATE_LONG | resultButton;       // ensure the button result reflects a long press
  } else if (timeoutShort && Released) {                          // short timeout has occurred (and not long timeout) and button was just released
      resultButton = STATE_SHORT | resultButton;     // ensure the button result reflects a short press
  } else {                                                                                  // else there is no change in status, return the normal state
      resultButton = STATE_NORMAL | resultButton; // with no change in status, ensure no change in button status
  }
}