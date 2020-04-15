#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>
HardwareTimer timer(2);



#define TIMER_RATE 1000
#define STATE_NORMAL 0
#define STATE_SHORT 1
#define STATE_LONG 2
#define NOISE 10 // Noise/hum/interference in mic signal





const PROGMEM uint16_t FullSine_6Bit[512] =
{
2048,2073,2098,2123,2148,2173,2198,2223,
2248,2273,2298,2323,2348,2373,2398,2422,
2447,2472,2496,2521,2545,2569,2594,2618,
2642,2666,2690,2714,2737,2761,2784,2808,
2831,2854,2877,2900,2923,2946,2968,2990,
3013,3035,3057,3078,3100,3122,3143,3164,
3185,3206,3226,3247,3267,3287,3307,3327,
3346,3366,3385,3404,3423,3441,3459,3477,
3495,3513,3530,3548,3565,3581,3598,3614,
3630,3646,3662,3677,3692,3707,3722,3736,
3750,3764,3777,3791,3804,3816,3829,3841,
3853,3865,3876,3888,3898,3909,3919,3929,
3939,3949,3958,3967,3975,3984,3992,3999,
4007,4014,4021,4027,4034,4040,4045,4051,
4056,4060,4065,4069,4073,4076,4080,4083,
4085,4087,4089,4091,4093,4094,4094,4095,
4095,4095,4094,4094,4093,4091,4089,4087,
4085,4083,4080,4076,4073,4069,4065,4060,
4056,4051,4045,4040,4034,4027,4021,4014,
4007,3999,3992,3984,3975,3967,3958,3949,
3939,3929,3919,3909,3898,3888,3876,3865,
3853,3841,3829,3816,3804,3791,3777,3764,
3750,3736,3722,3707,3692,3677,3662,3646,
3630,3614,3598,3581,3565,3548,3530,3513,
3495,3477,3459,3441,3423,3404,3385,3366,
3346,3327,3307,3287,3267,3247,3226,3206,
3185,3164,3143,3122,3100,3078,3057,3035,
3013,2990,2968,2946,2923,2900,2877,2854,
2831,2808,2784,2761,2737,2714,2690,2666,
2642,2618,2594,2569,2545,2521,2496,2472,
2447,2422,2398,2373,2348,2323,2298,2273,
2248,2223,2198,2173,2148,2123,2098,2073,
2048,2022,1997,1972,1947,1922,1897,1872,
1847,1822,1797,1772,1747,1722,1697,1673,
1648,1623,1599,1574,1550,1526,1501,1477,
1453,1429,1405,1381,1358,1334,1311,1287,
1264,1241,1218,1195,1172,1149,1127,1105,
1082,1060,1038,1017,995,973,952,931,
910,889,869,848,828,808,788,768,
749,729,710,691,672,654,636,618,
600,582,565,547,530,514,497,481,
465,449,433,418,403,388,373,359,
345,331,318,304,291,279,266,254,
242,230,219,207,197,186,176,166,
156,146,137,128,120,111,103,96,
88,81,74,68,61,55,50,44,
39,35,30,26,22,19,15,12,
10,8,6,4,2,1,1,0,
0,0,1,1,2,4,6,8,
10,12,15,19,22,26,30,35,
39,44,50,55,61,68,74,81,
88,96,103,111,120,128,137,146,
156,166,176,186,197,207,219,230,
242,254,266,279,291,304,318,331,
345,359,373,388,403,418,433,449,
465,481,497,514,530,547,565,582,
600,618,636,654,672,691,710,729,
749,768,788,808,828,848,869,889,
910,931,952,973,995,1017,1038,1060,
1082,1105,1127,1149,1172,1195,1218,1241,
1264,1287,1311,1334,1358,1381,1405,1429,
1453,1477,1501,1526,1550,1574,1599,1623,
1648,1673,1697,1722,1747,1772,1797,1822,
1847,1872,1897,1922,1947,1972,1997,2022,
   
};

volatile int  resultButton = 0; // global value set by checkButton()
volatile int  led = 0; 
int profilearray[13];
////do not define pins within global!!!!!poo on the shoe
//-->DECLARE  THINGS
long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
int i;
int b;
int p;
int poo;
int doo;
int a;

int state = 0;


float depth;
int centre;
float move_depth;
int hate;
float RAWIN;
float AWIN;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
float env;
int y;
int h;
float multiply = 1.00;
int release_f;
float attack_f;

bool vol = false;
bool dol = false;



           //running total of sample values
const int BENVA_NUMSAMPLE = 200;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int BENVA_SAMP[BENVA_NUMSAMPLE] = {0}; //value of sample
int BENVA_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int BENVA_TOTAL = 0;                   //running total of sample values
int test = 0;
float Master = 1.00;

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
    // -->SET UP PIN MODES
  
  
  
  
  //////INTRUPTS
  pinMode(PB13, INPUT_PULLUP);
  attachInterrupt(PB13, checkButton, CHANGE); // or rising / or change CHANGEPRESET
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, ONANDOFF, FALLING); // or rising / or change ONANDOFF
  // -->END SET UP PIN MODES
  //////INTRUPTS



///TIMER FOR CHECKING POTS
  //setupstm timer
  timer.pause();
  timer.setPeriod(TIMER_RATE); // in microseconds
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1); // Interrupt 1 count after each update
  timer.attachCompare1Interrupt(handler);
  timer.refresh();
  timer.resume();
///TIMER FOR CHECKING POTS



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
  //GET_OPENING_PROFILES();
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
 


if (vol){
poo=poo+ (attack_f+int(AWIN/10));
if (poo>512){poo=0;}
RAWIN = RAWIN + (((pgm_read_word(&(FullSine_6Bit[poo]))))/100);
}else{
//gate  

}

if (dol){
doo=doo+(attack_f);
if (doo>512){doo=0;}
Master = (((pgm_read_word(&(FullSine_6Bit[doo]))))/100);
}else{
//gate  
Master =1;
}
  

 */ 
  OUT1 = ((profilearray[3]) - ((profilearray[2] * depth))-Master); //HP
  OUT2 = ((profilearray[5]) - ((profilearray[4] * depth))-Master);
  OUT3 = ((profilearray[7]) - ((profilearray[6] * depth))-Master);
  


 if (profilearray[8]==1){
  OUT4 = centre + (RAWIN );
}else{  
  OUT4 = centre + ((0-RAWIN));}


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

}

void handler(void)
{
  checkthings();
}

void checkthings()
{

  /*RELEASE */
  release_f = analogRead(PA2); // change to relase / smoothing
  release_f = map(release_f, 0, 4095, 50, 500);
  /*ATTACK */
  attack_f = analogRead(PA4); // attack
  attack_f= map(attack_f, 0, 4095, 0, 10);


  /*DEPTH */
  depth = analogRead(PA3); // depth
  depth = map(depth, 0, 4095, 1, 100);
  depth = depth / 100;
  /*SENSITIVITY */
  move_depth = analogRead(PA1); // chanege to sens
 move_depth = map(move_depth, 0, 4095, 1, 100);
 move_depth =  move_depth/100;
 
  /*AUDIO IN */
  /*CENTRE FREQUENCY */
  centre = analogRead(PA5); //centre freq
  centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);
  /*AUDIO IN */
  RAWIN = analogRead(PA0); // AUDIOI;
  AWIN = RAWIN;
  BENVA_TOTAL -= BENVA_SAMP[BENVA_INDEX];   //Subtracts the last sample
  BENVA_SAMP[BENVA_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
  BENVA_TOTAL += BENVA_SAMP[BENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
  if (BENVA_INDEX >= (release_f))
  {                  //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }

  if (BENVA_INDEX >= BENVA_NUMSAMPLE - 2)
  {                  //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }

  RAWIN = (BENVA_TOTAL / (release_f));

  RAWIN = (RAWIN <= NOISE) ? 0 : (RAWIN - NOISE);
RAWIN = RAWIN * multiply;
 
}

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
  Q(4); //1, 3, 4, 2; 1 is strong
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