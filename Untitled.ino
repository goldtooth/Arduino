#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>

HardwareTimer timer(2);
#define TIMER_RATE 75

#define STATE_NORMAL 0
#define STATE_SHORT 1
#define STATE_LONG 2

// Button input related values
//static const int  STATE_NORMAL = 0; // no button activity
//static const int  STATE_SHORT  = 1; // short button press
//static const int  STATE_LONG  = 2; // long button press
volatile int  resultButton = 0; // global value set by checkButton()
volatile int  led = 0; 
int profilearray[13];
////do not define pins within global!!!!!poo on the shoe
//-->DECLARE  THINGS
long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
int i;
int b;
int p;
int wavetablelength = 512;
int poo;
int doo;
int a;
float comp;
int rise;
int state = 0;
float previous_val;
float val;

int trigger = 0;
float depth;
int centre;
float move_depth = 0.00;
int hate;
float Threshold = 200;
float Threshold2 = 100;
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
float comp_adjust;
float freakValue = 500.00;
int y;
int h;
float multiply = 1.00;
//const int ledCount = 255;
float gatein;
float gatein2;
float gateout;
int lastPeak = 1;
int check = 0;
int digger;
int zx;
int xx;
float dolvol;
bool source;
int release_f;
float oldAWIN;
float oldoldAWIN;
int oldrelease = 6000;
float vol_depth;
float dol_depth;
float vom_depth;
int attack_r;
#define NOISE 10 // Noise/hum/interference in mic signal
float env_duty;
//-->DECLARE  THINGS
float total;
float TopLimit;
float beancounter = 0;
bool vol = false;
bool dol = false;
bool zero = true;
bool gate = false;
bool triggered = false;
bool peaked = false;
//----------------------------
//running total of sample values
const int BENVA_NUMSAMPLE = 2000;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int BENVA_SAMP[BENVA_NUMSAMPLE] = {0}; //value of sample
int BENVA_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int BENVA_TOTAL = 0;
//----------------------------
//----------------------------
//running total of sample values
const int SHORT1_AVG_SAMPLE = 500;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int SHORT1_AVG_SAMP[SHORT1_AVG_SAMPLE] = {0}; //value of sample
int SHORT1_AVG_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int SHORT1_AVG_TOTAL = 0;
//----------------------------
//----------------------------
//running total of sample values
const int SHORT2_AVG_SAMPLE = 500;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int SHORT2_AVG_SAMP[SHORT2_AVG_SAMPLE] = {0}; //value of sample
int SHORT2_AVG_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int SHORT2_AVG_TOTAL = 0;
//----------------------------


int BENVA_REMAINDER = 0;//
int test = 0;
float shorterval;
float VOLDOL = 0.00;
float Master = 1.00;
float Naster = 1.00;
int complight;
bool mode;
float  locationcompensation = 0;

//gate settings
int gate_on_response = 50; // difference between SHORT1 & 2 needed for gate to turn on 
int SHORT1_SMOOTHING = 20; // SHORT1 Smoothing
int SHORT2_SMOOTHING = 200; // SHORT2 Smoothing
int ROLLOFF = 10;  //SPEED THAT GATE SHUTS OFF (higher number = faster.)

//remove offset on Audio input
int input_bias = 600; //600; Minus from initial input to remove bias  / potential negative /bias swing on env follower
int gatedepthlo = 1000; // Scale of For Gate Pot  - LOW
int gatedepthhi = 2000; // Scale of For Gate Pot  - HIGH

//Compressor
float ratio = 0.5;

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
  
  
  toberemoved();
 GET_OPENING_PROFILES();
}

void loop()
{
 int longButton=0;
 

  switch (resultButton) {
  
  case STATE_NORMAL: {
  break;
  }

  case STATE_SHORT: {
  CYCLETHROUGH();
  resultButton=STATE_NORMAL;
  break;
  }

  case STATE_LONG: {
  CHANGE_STATUS();
  longButton++;
  resultButton=STATE_NORMAL;
  break;
  }
  }
 

//---------------------------Dynamics-------------------------------------------
if (!dol){ //compressor or gate
 //-------------------------COMPRESSOR-------------------------------------------
if(!vol){  //take input from (rawish) input or from smoothed attack signal
//Take input signal from attack / smoothing signal.
if(AWIN>vol_depth){comp_adjust = AWIN-(vol_depth+((AWIN-vol_depth)*ratio));}else{comp_adjust=0;}}
 else{
//Act like normal Compressor
if(SHORT2>vol_depth){comp_adjust = SHORT2-(vol_depth+((SHORT2-vol_depth)*ratio));}else{comp_adjust=0;}
 }
//adust for ripple on comp and pre_env setting
ENV_ADJUST = comp_adjust/2; 
}
else{

///-------------------------GATE------------------------------------------------
if((SHORT1-gate_on_response>=SHORT2)){                    // IF SHORT1 > SHORT2 TURN ON  
if(!gate){if(vol){comp_adjust = 2500;}else{comp_adjust = 0;}}
gate = true;}                                             // GATE ON IS CALLED

if(vol){                                                  //IF TAKING FROM ATTACK SIGNAL
if (gate){                                                //...AND GATE IS CALLED
comp_adjust = comp_adjust - (rise/10);                    //RISE VOLUME.
if (comp_adjust<=0){comp_adjust = 0;}                     //safety check
}}
                                                          //------------ALERT DO REALLY WANT RAWIN CONTROLLING THE RELEASE???/
if(gate && (RAWIN+2<dol_depth)){gate = false; }           //IF GATE IS ALLREADY ON AND SIGNAL FALLS BELOW TURN GATE OFF
if (!gate){comp_adjust = comp_adjust + ROLLOFF;}               //RELEASE / DECAY SPEED.    
if (comp_adjust>=4096){comp_adjust = 4096;}              // SAFETY CHECK
ENV_ADJUST = comp_adjust;
//GATE END------------------------------------------------
}


AWIN = AWIN + input_bias;
Master = comp_adjust;

//UP OR DOWN?
  if (profilearray[8]==1){AWIN = AWIN - ENV_ADJUST;}else{AWIN = 0.00-(AWIN+ENV_ADJUST);}

  OUT1 = ((4096-Master) - ((profilearray[2] * depth))); //HP
  OUT2 = ((4096-Master) - ((profilearray[4] * depth)));
  OUT3 = ((4096-Master) - ((profilearray[6] * depth)));
  OUT4 = (centre) + ((AWIN * move_depth));
  //Master = (profilearray[9]*depth);

  //THELISTENER ==true

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


  dac.analogWrite(OUT1, OUT2, OUT3, OUT4);
  //--------------- HP, MID, LP, FREQ
  //-------------4095 is off,  0 is full
}

void handler(void)
{
  checkthings();
}

void checkthings()
{

 
  //---------------------------------------------*ATTACK*------------------------------------------------------------------*/
  vol_depth = analogRead(PA4); // attack
  dol_depth = map(vol_depth, 0, 4095, gatedepthlo, gatedepthhi);
  vol_depth= map(vol_depth, 0, 4095, 2500, 0);
 //vol_depth = vol_depth / 100;

  //---------------------------------------------*DEPTH*------------------------------------------------------------------*/
  depth = analogRead(PA3); // depth
  depth = map(depth, 0, 4095, 0, 50);
  depth = depth / 100;

  //---------------------------------------------*RELEASE*------------------------------------------------------------------*/
  release_f = analogRead(PA2); // change to relase / smoothing
  rise = map(release_f, 0, 4095, 200, 10);
  release_f = map(release_f, 0, 4095, 20, 1900);

  //---------------------------------------------*ENVELOPE MOVEMENT /  SENSITIVITY*------------------------------------------------------------------*/
 move_depth = analogRead(PA1); // chanege to sens
 move_depth = map(move_depth, 0, 4095, 0, 200);
 move_depth =  move_depth/100;

  //---------------------------------------------*CENTRE FREQUENCY*------------------------------------------------------------------*/
centre = analogRead(PA5);                                           //centre freq
centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);

  //---------------------------------------------*AUDIO IN*------------------------------------------------------------------*/
RAWIN = analogRead(PA0);             
RAWIN = RAWIN - input_bias;                                         //RAWIN bias to remove dip / offset
if(RAWIN<=0){RAWIN = 0;}
RAWIN = map(RAWIN, 0, 2500, 0, 4096);

  //---------------------------------------------*AVERAGING*------------------------------------------------------------------*/
SHORT1_AVG_TOTAL -= SHORT1_AVG_SAMP[SHORT1_AVG_INDEX];   //Subtracts the last sample
SHORT1_AVG_SAMP[SHORT1_AVG_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
SHORT1_AVG_TOTAL += SHORT1_AVG_SAMP[SHORT1_AVG_INDEX++]; //adds the sample to the total and moves INDEX to the next value
SHORT2_AVG_TOTAL -= SHORT2_AVG_SAMP[SHORT2_AVG_INDEX];   //Subtracts the last sample
SHORT2_AVG_SAMP[SHORT2_AVG_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
SHORT2_AVG_TOTAL += SHORT2_AVG_SAMP[SHORT2_AVG_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (SHORT1_AVG_INDEX >= (SHORT1_SMOOTHING)){SHORT1_AVG_INDEX = 0;}
if (SHORT2_AVG_INDEX >= (SHORT2_SMOOTHING)){SHORT2_AVG_INDEX = 0;}
SHORT1 = SHORT1_AVG_TOTAL/SHORT1_SMOOTHING;
SHORT2 = SHORT2_AVG_TOTAL/SHORT2_SMOOTHING;
BENVA_TOTAL -= BENVA_SAMP[BENVA_INDEX];   //Subtracts the last sample
BENVA_SAMP[BENVA_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
BENVA_TOTAL += BENVA_SAMP[BENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
//------------------------------------ RESET BENVA_TOTAL when knob changes
if(abs(release_f - oldrelease)>20){BENVA_TOTAL=0;oldrelease = release_f;}
//------------------------------------
if (BENVA_INDEX >= (release_f)){BENVA_INDEX = 0;} // //checks if limit of samples is reached, resets INDEX to 0, restarting the process
if (BENVA_INDEX >= BENVA_NUMSAMPLE - 2){   BENVA_INDEX = 0; } ////just a double check to make sure it doesn't overrun the buffer
AWIN = (BENVA_TOTAL / (release_f));
AWIN = (AWIN <= NOISE) ? 0 : (AWIN - NOISE); //Do I really need the noise reduction? 
AWIN = AWIN * multiply; // Multiply added to take care of any compensation when switching between profiles.
}
//--------------------------------------- AVERAGING END----------------------------------------------
///ALL OTHER BORING STUFF

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
      oneblink();
      break;

    case 2:
      twoblink();
      break;

    case 3:
      threeblink();
      break;

    case 4:
      fourblink();
      break;

    case 5:
      fiveblink();
      break;

    case 6:
      sixblink();
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
   locationcompensation = 0.00;
  digitalWrite(PB14, LOW);
  digitalWrite(PB15, HIGH);
  digitalWrite(PA10, HIGH);
  }else{
 multiply = 2.00;
 locationcompensation = 0.00;
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


void oneblink(){int ledpin=PB5; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}
void twoblink(){int ledpin=PB4; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}
void threeblink(){int ledpin=PA11; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}
void fourblink(){int ledpin=PA10; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}
void fiveblink(){int ledpin=PA9; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}
void sixblink(){int ledpin=PA8; bool onstate = digitalRead(ledpin);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);digitalWrite(ledpin, !onstate);delay(100);digitalWrite(ledpin, onstate);delay(100);}



void GET_OPENING_PROFILES()
{
one();
two();
three();
four();
five();
six();
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
