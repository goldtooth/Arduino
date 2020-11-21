
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




int attack_f = 10;  // attack period (how soon the compressor will start attenuating loud signals) given in measurement frame
                    // units (see window_ms). Default setting corresponds to 50ms. Max buf_len / 2. Min 4.
int release_f = 40; // release period (how soon the compressor will soften attenuation after signals have become more silent),
                    // given in measurement frame units. Default setting corresponds to 200ms; Max buf_len.
                    // Does not have an effect if <= attack_f
int threshold = 18; // minimum signal amplitude before the compressor will kick in. Each unit corresponds to roughly 5mV
                    // peak-to-peak.
float ratio = 3.0;  // dampening applied to signals exceeding the threshold. n corresponds to limiting the signal to a level of
                    // threshold level plus 1/3 of the level in excess of the threshold (if possible: see duty_min, below)
                    // 1(min) = no attenuation; 20(max), essentially limit to threshold, aggressively
const float max_transition_rate = 1.11; // although the moving averages for attack and release will result in smooth transitions
                    // of the compression rate in most regular cases sudden signal spikes can result in abrupt transitions, introducing
                    // additional artefacts. This limits the maximum speed of the transition to +/- 11% of current value.

//// Some further constants that you will probably not have to tweak ////
#define DEBUG 1           // serial communication appears to introduce audible noise ("ticks"), thus debugging is diabled by default
const int window_ms = 5;  // milliseconds per measurement window. A narrow window will allow finer control over attack and release,
                    // but it will also cripple detection of low frequency amplitudes. Probably you don't want to change this.
const int buf_len = 100;  // size of buffer. attack_f and release_f cannot exceed this.
const int duty_min = 10;  // ceiling value for attenuation (lower values = more attenuation, 0 = off, 255 = no attenuation)
                    // beyond a certain value further attenuation is just too coarse grained for good results. Ideally, this
                    // value is never reached, but might be for aggressive dampening ratio and low thresholds.
const int duty_warn = 2 * duty_min;  // See above. At attenuation beyond this (i.e. smaller numbers), warning LED will flash.
                    // Reaching this point on occasion is quite benign. Reaching this point much of the time means too strong
                    // signal, too low threshold setting, or too aggressive inv_ratio.



//// working variables ////
volatile int cmin = 1024; // minimum amplitude found in current measurement window
volatile int cmax = 0;    // maximum amplitude found in current measurement window
int buf[buf_len];         // ring buffer for moving averages / sums
int pos = 0;              // current buffer position
int attack_mova = 0;      // moving average (actually sum) of amplitudes over past attack period
int release_mova = 0;     // moving average (actually sum) of amplitudes over past release period
int32_t now = 0;          // start time of current loop
int32_t last = 0;         // time of last loop
int duty = 255;           // current PWM duty cycle for attenuator switch(es) (0: hard off, 255: no attenuation)
byte display_hold = 0;
float invratio = 1 / ratio;  // inverse of ratio. Saves some floating point divisions


/*** Handle new analog readings as they become available. This simply records the highest and lowest voltages seen in the current
     measurement window. All real (and more computation-heavy) handling is done inside loop(). ***/
ISR(ADC_vect) {
  int aval = ADCL;    // store lower byte ADC
  aval += ADCH << 8;  // store higher byte ADC
  if (aval < cmin) cmin = aval;
  if (aval > cmax) cmax = aval;
}

void setup() {
  for (int i = 0; i < buf_len; ++i) {  // clear buffer
    buf[i] = 0;
  }



  // setup fast continuous analog input sampling. Kudos go to https://meettechniek.info/embedded/arduino-analog.html
  // whenever a new reading is available, the routine defined by ISR(ADC_vect) is called.
  DIDR0 = 0x3F;            // digital input buffers disabled on all analog pins
  ADMUX = 0b01000000;      // measuring on ADC0, use 5v reference
  ADCSRA = 0xAC;           // AD-converter on, interrupt enabled, prescaler = 16  --> around 77k samples per second
  ADCSRB = 0x40;           // AD channels MUX on, free running mode
  bitWrite(ADCSRA, 6, 1);  // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
  sei();                   // set interrupt flag

  last = millis();




}

void loop() {
  now = millis();
  if (now < last || now - last > window_ms) {  // measurment window elapsed (or timer overflow)
    last = now;
  } else return;



  // get amplitude in current meausrement window, and set up next window
  if (++pos >= buf_len) pos = 0;
  int val = cmax - cmin;
  if (val < 0) val = 0;
  cmax = 0;
  cmin = 1024;

  // update the two moving averages (sums)
  int old_pos = pos - attack_f;
  if (old_pos < 0) old_pos += buf_len;
  attack_mova += val - buf[old_pos];
  old_pos = pos - release_f;
  if (old_pos < 0) old_pos += buf_len;
  release_mova += val - buf[old_pos];

  // store new value in ring buffer
  buf[pos] = val;

  // calculate new attenuation settings
  // first caculate based on attack period
  const int attack_threshold = threshold * attack_f;
  int attack_duty = 4096;
  if (attack_mova > attack_threshold) {
    const int target_level = attack_threshold * pow ((float) attack_mova / attack_threshold, invratio);
// Instead of the logrithmic volume calculation above, the faster linear one below seems too yield
// acceptable results, too. Hoever, the Arduino is fast enough, so we do the "real" thing.
//   const int target_level = (attack_mova - attack_threshold) / ratio + attack_threshold;
    attack_duty = (4096 * (int32_t) target_level) / attack_mova;

  }
  // if the new duty setting is _below_ the current, based on attack period, check release window to see, if
  // the time has come to release attenuation, yet:
  if (attack_duty < duty) duty = max (attack_duty, duty / max_transition_rate);
  else {
    int release_duty = 4096;
    const int release_threshold = threshold * release_f;
    if (release_mova > release_threshold) {
      const int target_level = release_threshold * pow ((float) release_mova / release_threshold, invratio);
      release_duty = (4096 * (int32_t) target_level) / release_mova;
    } else {
      release_duty = 4096;
    }
    if (release_duty >= duty) duty = min (release_duty, duty * max_transition_rate);

  }

 dac.output(duty); // enable the new duty cycle

}
void checkthings(){
  //read depth pot and scale
  attack = analogRead(depthpot);
  attack = map(attack , 0, 1023, 0, 2000);
  attack = attack/1000;

  //read rate pot and add delay to make longer=
  release = analogRead(ratepot);
  release  = map(release , 0, 1023, 5, 1000); //last value here is the lowest frequency  (max 1023)
  //rate = fscale( 0, 1023, 100, 2000, rate, -4);
  
  Threshold = analogRead(shiftpot);
  Threshold  = map(Threshold , 0, 1023, 750, 0);
   //Threshold = Threshold*4;
RAWIN = analogRead(audioin);
//RAWIN = map(RAWIN , 1, 1023, 1, 1023); //last value here is the lowest frequency  (max 1023)
  //rate = fscale( 0, 1023, 100, 2000, rate, -4);  


    


     
                                   //RAWIN bias to remove dip / offset
//if(rms){RAWIN = RAWIN * 0.7071;}   //RMS                                  
if(RAWIN<=0){RAWIN = 0;}

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