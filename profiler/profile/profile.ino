#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>

HardwareTimer timer(2);
#define TIMER_RATE 75

#define STATE_NORMAL 0 //static const int  STATE_NORMAL = 0; // no button activity
#define STATE_SHORT 1 //static const int  STATE_SHORT  = 1; // short button press
#define STATE_LONG 2 //static const int  STATE_LONG  = 2; // long button press
volatile int  resultButton = 0; // global value set by checkButton()
volatile int  led = 0; 

long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
int profilearray[13];
int i;
int b;
int p;
int a;
int rise;
int state = 0;
float depth;
int centre;
float move_depth = 0.00;
int hate;
int RAWIN;
float AWIN;
float SHORT1;
float SHORT2;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
float env;
float ENV_ADJUST;
float adjust;
int y;
int h;
float multiply = 1.00;
//const int ledCount = 255;
float gatein;
float gatein2;
float gateout;
int release_f;
int oldrelease = 6000;
float vol_depth;
float dol_depth;
float vom_depth;
int attack_r;
#define NOISE 10 // Noise/hum/interference in mic signal
float env_duty;
//-->DECLARE  THINGS
bool vol = false;
bool dol = false;
bool gate = false;


float  Threshold;
bool comp_status_on;
float attack;
float release;
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
float ratio = 0.5;
float working_ratio;
//----------------------------
//running total of sample values

float Master = 1.00;
bool mode;

//remove offset on Audio input
int input_bias = 600; //600; Minus from initial input to remove bias  / potential negative /bias swing on env follower
int gatedepthlo = 500; // Scale of For Gate Pot  - LOW
int gatedepthhi = 2000; // Scale of For Gate Pot  - HIGH



volatile byte tate = HIGH;
void setup()
{
  disableDebugPorts();
  dac.begin();   // initialize i2c interface
  dac.vdd(3300); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(0, 0, 0, 0);
  //dac.setVref(1,1,1,1);

  // -->SET UP PIN MODES

  pinMode(PA8, OUTPUT);  //LED
  pinMode(PA9, OUTPUT);  //LED
  pinMode(PA10, OUTPUT); //LED
  pinMode(PA11, OUTPUT); //LED
  pinMode(PB4, OUTPUT);  //LED
  pinMode(PB5, OUTPUT);  //LED
  pinMode(PB8, OUTPUT);  //q
  pinMode(PB9, OUTPUT);  //q
  pinMode(PB14, OUTPUT); // ENV DETECT input selection
  pinMode(PB15, OUTPUT); // ENV DETECT input selection
  pinMode(PA12, OUTPUT); //SS_relay
  pinMode(PA15, OUTPUT); //relay
  pinMode(PA0, INPUT_ANALOG);
  pinMode(PA1, INPUT); //POTS
  pinMode(PA2, INPUT); //POTS
  pinMode(PA3, INPUT); //POTS
  pinMode(PA4, INPUT); //POTS
  pinMode(PA5, INPUT); //POTS
  pinMode(PA6, INPUT); //ADDITIONAL POT IF NO PLUGIN
  // Setup the second button with an internal pull-up :
  pinMode(PB13, INPUT_PULLUP);
  attachInterrupt(PB13, checkButton, CHANGE); // or rising / or change CHANGEPRESET
  
  
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, ONANDOFF, FALLING); // or rising / or change ONANDOFF
  // -->END SET UP PIN MODES





  //setupstm timer
  timer.pause();
  timer.setPeriod(TIMER_RATE); // in microseconds
      // Set up an interrupt on channel 1
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1); // Interrupt 1 count after each update
  timer.attachCompare1Interrupt(handler);
  timer.refresh();
  timer.resume();

  digitalWrite(PB8, LOW);
  digitalWrite(PB9, LOW);
  digitalWrite(PA8, LOW);
  digitalWrite(PA9, LOW);
  digitalWrite(PA10, LOW);
  digitalWrite(PA11, LOW);
  digitalWrite(PB4, LOW);
  digitalWrite(PB5, LOW);

  //ENVELOPE FOLLOWER SELECTION
  digitalWrite(PB15, LOW);
  digitalWrite(PB14, HIGH);
  
  
 //toberemoved();
 GET_OPENING_PROFILES();
}

void loop()
{

if((SHORT1>Threshold)) // if short is over the threashold
{
if(!comp_status_on){comp_status_on = true; digitalWrite(PA8, HIGH);working_ratio = 1;} //if status is off, turn it on, turn on LED
working_ratio = working_ratio - attack;
if(working_ratio <= ratio){working_ratio=ratio;}
adjust = SHORT1-(Threshold+((SHORT1-Threshold)*working_ratio)); //what is in the brackets is the compressed signal, total equation is the difference.
}else{
if(comp_status_on){adjust = adjust-(release/5000);} 
if (adjust <= 0){adjust=0; comp_status_on = false; digitalWrite(PA8, LOW);}
 }


//AT thia point what is the ideal values  - 
  //---------------------------------------------*CONDITION SIGNALS TO OUTPUTS
  OUT1 = ((4096-Master) - ((profilearray[2] * depth))); //HP
  OUT2 = ((4096-Master) - ((profilearray[4] * depth)));
  OUT3 = ((4096-Master) - ((profilearray[6] * depth)));
  OUT4 = (centre) + ((AWIN * move_depth));


  if (OUT1 > 4095){OUT1 = 4095;}
  if (OUT1 < 1){OUT1 = 0;}
  if (OUT2 > 4095){OUT2 = 4095;}
  if (OUT2 < 1){OUT2 = 0;}
  if (OUT3 > 4095){OUT3 = 4095;}
  if (OUT3 < 1){OUT3 = 0;}
  if (OUT4 > 4095){OUT4 = 4095;}
  if (OUT4 < 1){ OUT4 = 0;}

  OUT1 = ((0 - OUT1) + 4095);
  OUT2 = ((0 - OUT2) + 4095);
  OUT3 = ((0 - OUT3) + 4095);
  OUT4 = ((0 - OUT4) + 4095);


  dac.analogWrite(OUT1, OUT2, OUT3, OUT4);//--------------- HP, MID, LP, FREQ//-------------4095 is off,  0 is full
}

void handler(void){checkthings();}

void checkthings()
{
  //---------------------------------------------*THRESHOLD*------------------------------------------------------------------*/
  Threshold = analogRead(PA4);
  Threshold  = map(Threshold , 0, 1023, 750, 0);

  //---------------------------------------------*DEPTH*------------------------------------------------------------------*/
release = analogRead(PA3);
release  = map(release , 0, 4095, 5, 1000); //last value here is the lowest frequency  (max 1023)
  //rate = fscale( 0, 1023, 100, 2000, rate, -4);

  //---------------------------------------------*ATTACK*------------------------------------------------------------------*/
  //
  attack = analogRead(PA2);
  attack = map(attack , 0, 4095, 0, 2000);
  attack = attack/1000;

  //---------------------------------------------*ENVELOPE MOVEMENT /  SENSITIVITY*------------------------------------------------------------------*/
 move_depth = analogRead(PA1); // chanege to sens
 move_depth = map(move_depth, 0, 4095, 0, 200);
 move_depth =  move_depth/100;

  //---------------------------------------------*CENTRE FREQUENCY*------------------------------------------------------------------*/
centre = analogRead(PA5);                                           //centre freq
centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);

  //---------------------------------------------*AUDIO IN*------------------------------------------------------------------*/
RAWIN = RAWIN * 0.7071;   //RMS                                  
if(RAWIN<=0){RAWIN = 0;}
RAWIN = RAWIN - input_bias;

RAWIN = map(RAWIN, 0, (1023-input_bias), 0, 1023);

  //---------------------------------------------*AVERAGING*------------------------------------------------------------------*/
SHORT1_AVG_TOTAL -= SHORT1_AVG_SAMP[SHORT1_AVG_INDEX];   //Subtracts the last sample
SHORT1_AVG_SAMP[SHORT1_AVG_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
SHORT1_AVG_TOTAL += SHORT1_AVG_SAMP[SHORT1_AVG_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (SHORT1_AVG_INDEX >= (SHORT1_SMOOTHING)){SHORT1_AVG_INDEX = 0;}
SHORT1 = SHORT1_AVG_TOTAL/SHORT1_SMOOTHING;
}


void ONANDOFF()
{
  if ((long)(micros() - last_micros) >= debouncing_time * 1500)
  {
    TOGGLE_SS_RELAY();
    TOGGLE_HW_RELAY();
    last_micros = micros();
  }
}

void TOGGLE_HW_RELAY()
{
  if (state == 0)
  {
    digitalWrite(PA15, HIGH);
    // PORTA |= _BV(PA15); //8
    state = 1;
  }
  else
  {
    digitalWrite(PA15, LOW);
    // PORTA &= ~_BV(PA15); //8
    state = 0;
  }
  TOGGLE_SS_RELAY();
}

void TOGGLE_SS_RELAY()
{
  if (hate == 0){digitalWrite(PA12, HIGH);hate = 1;}
  else{digitalWrite(PA12, LOW);hate = 0;}
}


void CHANGE_STATUS(){

if(led == 1){one();}
if(led == 2){two();}
if(led == 3){three();}
if(led == 4){four();}
if(led == 5){five();}
if(led == 6){six();}



}


void CYCLETHROUGH()
{

      led++;
      if (led >= 7)
      {
        led = 1;
      }

    switch (led)
    {
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

/* Q SETTINGS */
void Q(int cally)
{
  if (cally == 1)
  {
    digitalWrite(PB8, LOW);
    digitalWrite(PB9, LOW);
  }
  if (cally == 2)
  {
    digitalWrite(PB8, HIGH);
    digitalWrite(PB9, LOW);
  }
  if (cally == 3)
  {
    digitalWrite(PB8, LOW);
    digitalWrite(PB9, HIGH);
  }
  if (cally == 4)
  {
    digitalWrite(PB8, HIGH);
    digitalWrite(PB9, HIGH);
  }
}



void six()
{
  bool sixstate = digitalRead(PA8);
  if(sixstate==LOW){
  digitalWrite(PA8, HIGH);
 vol = true;
  }else{

digitalWrite(PA8, LOW);
vol = false;

  }
}

void five()
{
  /*-------------------------------------------------------------------------------*/
 bool fivestate = digitalRead(PA9);
   if(fivestate==LOW){
   digitalWrite(PA9, HIGH);
 dol = true;
  }else{ 
 dol = false;
  digitalWrite(PA9, LOW);


  }
 
 

}

void four()
{
  /*-------------------------------------------------------------------------------*/
  bool fourstate = digitalRead(PA10);
  if(fourstate==LOW){
    multiply = 1.00;
  digitalWrite(PB14, LOW);
  digitalWrite(PB15, HIGH);
  digitalWrite(PA10, HIGH);
  }else{
 multiply = 2.00;
digitalWrite(PB14, HIGH);
digitalWrite(PB15, LOW);
digitalWrite(PA10, LOW);

  }

}

void three()
{
  /*-------------------------------------------------------------------------------*/
    bool threestate = digitalRead(PA11);
  if(threestate==LOW){
  profilearray[2] = 0; //4096-env_duty; //HP CENTRE VOLUME
  profilearray[4] = 4096; // BP CENTRE VOLUME
  profilearray[6] = 0; // LP CENTRE VOLUME
  digitalWrite(PA11, HIGH);

  }else{
  profilearray[2] = 4096; //4096-env_duty; //HP CENTRE VOLUME
  profilearray[4] = 0; // BP CENTRE VOLUME
  profilearray[6] = 4096; // LP CENTRE VOLUME
digitalWrite(PA11, LOW);

  }



}

void two()
{

  bool twostate = digitalRead(PB4);
  if(twostate==LOW){
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PB4, HIGH);
  Q(1); //1, 3, 4, 2; 1 is strong
  }else{
 digitalWrite(PB4, LOW);
  Q(2); //1, 3, 4, 2; 1 is strong
  }

}

void one()
{
  /*-------------------------------------------------------------------------------*/
 bool onestate = digitalRead(PB5);
  if(onestate==LOW){
  digitalWrite(PB5, HIGH);
  profilearray[8] = 1;
  }else{
  profilearray[8] = 0;
  digitalWrite(PB5, LOW); 
  }
}



void GET_OPENING_PROFILES()
{
led=0;
}


void toberemoved(){
    Q(4); //1, 3, 4, 2; 1 is stronge
    profilearray[1] = 500; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 100; //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 4096; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 4096;    //  HP Profile Variable or number

    profilearray[4] = 0; // BP CENTRE VOLUME
    profilearray[5] = 4096; //env_duty; //BP Profile Variable or number

    profilearray[6] = 4096; // LP CENTRE VOLUME
    profilearray[7] = 4096;    // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 4095; //higher scope of centre frequency knob*4
    profilearray[11] = 500;  //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = 0;    //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 2500; // top of the audio envelope
    profilearray[12] = 0;   // BOttom of the audio envelope

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

  buttonStatus = digitalRead(PB13);                // read the button state on the pin "BUTTON_PIN"
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
