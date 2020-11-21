#include <mcp4728.h>
#include </Users/nickpacker/Documents/Arduino/hardware/Arduino_STM32/STM32F1/libraries/Wire/Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>
#include "wavetable.h"

HardwareTimer timer(2);
#define TIMER_RATE 1000

////do not define pins within global!!!!!poo on the shoe
//-->DECLARE  THINGS
long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
int led = 1;
int i;
int b;
int p;
int a;
float comp;
//uint16_t h;
int state = 0;
float previous_val;
float val;
int profilearray[13];
int trigger = 0;
float depth;
int centre;
float sens;
int hate;
int Threshold = 500;
float RAWIN;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
float env;
float freakValue = 500.00;
int y;
int h;

//const int ledCount = 255;
const int numReadings = 20;
int readings[numReadings];
float peakValue = 1;
float beakValue = 1;
int lastPeak = 1;
int check = 0;
int digger;
float release_f;
int attack_f;
int attack_r;
#define NOISE 10 // Noise/hum/interference in mic signal
float env_duty;
//-->DECLARE  THINGS
int count;
float total;
float TopLimit;
float beancounter = 0;
bool zero = true;
bool triggered = false;
bool peaked = false;
const int GENVA_NUMSAMPLE = 1000; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int GENVA_SAMP[GENVA_NUMSAMPLE] = {0}; //value of sample
int GENVA_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int GENVA_TOTAL = 0;                   //running total of sample values
const int ENVA_NUMSAMPLE = 20;         //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int ENVA_SAMP[ENVA_NUMSAMPLE] = {0}; //value of sample
int ENVA_INDEX = 0;                  //number of sample from 1-[NUMSAMPLE]
int ENVA_TOTAL = 0;                  //running total of sample values
const int BENVA_NUMSAMPLE = 100;     //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int BENVA_SAMP[BENVA_NUMSAMPLE] = {0}; //value of sample
int BENVA_INDEX = 0;                   //number of sample from 1-[NUMSAMPLE]
int BENVA_TOTAL = 0;                   //running total of sample values
int test = 0;
float shorterval;
float oldpeak = 0.0;
float Master;
int complight;
bool mode;
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
  attachInterrupt(PB13, CHANGEPRESET, RISING); // or rising / or change CHANGEPRESET
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
  digitalWrite(PB14, LOW);
  digitalWrite(PB15, HIGH);

  one();
}

void loop()
{

  //limit or compress the envelope

  //create a compressor as a profile - either increase or decrease vol
  //int comp_thresh=200;
  //if(peakValue>comp_thresh){comp = 0-(peakValue-comp_thresh);}else{comp=(comp_thresh-peakValue);}

  //comp
  complight = int(comp / 100);

  //create env_related profile for wavetables.
  trigger = peakValue;
  if (trigger > 512)
  {
    trigger = 512;
  }

  //Main profile.
  env_duty = peakValue; // +
  env_duty = map(env_duty, 0, 1500, profilearray[12], profilearray[0]);

  y++;
  if (y >= 64)
  {
    y = 0;
  }
  h = (gate[y]);
  p = (gate[y + 32]);
  GET_PROFILES();

  Master = 1 - (((1 - 2048) / 2048));
  //? is that 1

  OUT1 = (profilearray[2] + (profilearray[3] * depth)) * Master; //HP
  OUT2 = (profilearray[4] + (profilearray[5] * depth)) * Master;
  OUT3 = (profilearray[6] + (profilearray[7] * depth)) * Master;
  OUT4 = centre + (profilearray[8] * depth);
  //Master = (profilearray[9]*depth);

  //THELISTENER ==true

  if (OUT1 > 4095)
  {
    OUT1 = 4095;
  }
  if (OUT1 < 1)
  {
    OUT1 = 0;
  }
  if (OUT2 > 4095)
  {
    OUT2 = 4095;
  }
  if (OUT2 < 1)
  {
    OUT2 = 0;
  }
  if (OUT3 > 4095)
  {
    OUT3 = 4095;
  }
  if (OUT3 < 1)
  {
    OUT3 = 0;
  }
  if (OUT4 > 4095)
  {
    OUT4 = 4095;
  }
  if (OUT4 < 1)
  {
    OUT4 = 0;
  }

  OUT1 = ((0 - OUT1) + 4095);
  OUT2 = ((0 - OUT2) + 4095);
  OUT3 = ((0 - OUT3) + 4095);
  OUT4 = ((0 - OUT4) + 4095);

  //delayMicroseconds(actdelay);
  dac.analogWrite(OUT1, OUT2, OUT3, OUT4);
  //--------------- HP, MID, LP, FREQ
  //-------------4095 is off,  0 is full
}

void handler(void)
{
  checkthings();
};

void checkthings()
{

  /*RELEASE */
  release_f = analogRead(PA2); // change to relase / smoothing
  release_f = map(release_f, 0, 4095, 50, 1);
  /*ATTACK */
  attack_f = analogRead(PA4); // attack
  attack_r = map(attack_f, 0, 4095, 600, 4);
  attack_f = map(attack_f, 0, 4095, 4, 600);

  /*DEPTH */
  depth = analogRead(PA3); // depth
  depth = map(depth, 0, 4095, 1, 100);
  depth = depth / 100;
  /*SENSITIVITY */
  sens = analogRead(PA1); // chanege to sens
  /*CENTRE FREQUENCY */
  centre = analogRead(PA5); //centre freq
  centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);
  /*AUDIO IN */
  RAWIN = analogRead(PA0); // AUDIOI;

  //compress signal a little.
  //if (RAWIN > 500){RAWIN=500*(1.0+(50/100));}
  //if RAWIN is over threashold then aplly ratio (threshold plus ratio in percentage terms (/100))

  ENVA_TOTAL -= ENVA_SAMP[ENVA_INDEX];   //Subtracts the last sample
  ENVA_SAMP[ENVA_INDEX] = RAWIN;         //sets the sample to the analoge reading (0-1024)
  ENVA_TOTAL += ENVA_SAMP[ENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
  if (ENVA_INDEX >= 20)
  {                 //checks if limit of samples is reached
    ENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  ////just a double check to make sure it doesn't overrun the buffer
  ////////////////////////////////////////////////////////////////////////
  if (ENVA_INDEX >= ENVA_NUMSAMPLE - 2)
  {                 //checks if limit of samples is reached
    ENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  ////////////////////////////////////////////////////////////////////////
  ////just a double check to make sure it doesn't overrun the buffer

  previous_val = ENVA_SAMP[ENVA_INDEX - 5];
  val = ((ENVA_TOTAL / 20));

  //if(val>profilearray[1]){val = val-(val-profilearray[1]);}else{}
  //RAWIN = map(RAWIN,0,4096, 0, sens*2);

  BENVA_TOTAL -= BENVA_SAMP[BENVA_INDEX];   //Subtracts the last sample
  BENVA_SAMP[BENVA_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
  BENVA_TOTAL += BENVA_SAMP[BENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
  if (BENVA_INDEX >= (100))
  {                  //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  ////just a double check to make sure it doesn't overrun the buffer
  ////////////////////////////////////////////////////////////////////////
  if (BENVA_INDEX >= BENVA_NUMSAMPLE - 2)
  {                  //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  ////////////////////////////////////////////////////////////////////////
  ////just a double check to make sure it doesn't overrun the buffer
  shorterval = (BENVA_TOTAL / (100));

  digger = GENVA_SAMP[GENVA_INDEX];
  GENVA_TOTAL -= digger;                    //Subtracts the last sample
  GENVA_SAMP[GENVA_INDEX] = RAWIN;          //sets the sample to the analoge reading (0-1024)
  GENVA_TOTAL += GENVA_SAMP[GENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
  if (GENVA_INDEX >= (attack_f))
  {                  //checks if limit of samples is reached
    GENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  ////just a double check to make sure it doesn't overrun the buffer
  ////////////////////////////////////////////////////////////////////////
  if (GENVA_INDEX >= GENVA_NUMSAMPLE - 2)
  {                  //checks if limit of samples is reached
    GENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }

  val = (val <= NOISE) ? 0 : (val - NOISE); // Remove noise/hum
  RAWIN = (RAWIN <= NOISE) ? 0 : (RAWIN - NOISE);
  TopLimit = 500.00;
  //downward cycle

  if ((RAWIN > (val * 1.2)) && (val > (shorterval * 1.5)))
  {
    digitalWrite(PA8, HIGH);
    beancounter = 0;
  }
  else
  {
    digitalWrite(PA8, LOW);
  }

  if (beancounter <= TopLimit)
  {
    beancounter = beancounter + (attack_f / 10);
    peakValue = beancounter;
  }
  else
  {
    beancounter = beancounter + (release_f / 2);
    peakValue = (TopLimit * 2) - beancounter;
  }

  if (beancounter >= (TopLimit * 2))
  {
    beancounter = (TopLimit * 2);
  }

  if (RAWIN <= val)
  {
    beakValue = beakValue - (release_f / 50);
    if (beakValue <= 0)
    {
      beakValue = 0;
    }
    total = beakValue * attack_r;
  }
  else
  {
    total = ((total + RAWIN) - GENVA_SAMP[GENVA_INDEX]);
    beakValue = (total / attack_r);
    if (beakValue > profilearray[1])
    {
      beakValue = ((float)profilearray[1] * (1.0 + ((float)profilearray[9] / 100)));
    }
  }

  //if (beakValue>peakValue){peakValue=beakValue;}
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
  if (hate == 0)
  {

    digitalWrite(PA12, HIGH);
    hate = 1;
  }
  else
  {
    digitalWrite(PA12, LOW);
    hate = 0;
  }
}

void CHANGEPRESET()
{

  if ((long)(micros() - last_micros2) >= debouncing_time * 2500)
  {

    if (test == 1 || test == 3)
    {
    }
    else
    {
      led++;
      if (led >= 7)
      {
        led = 1;
      }
      last_micros2 = micros();
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
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PA8, HIGH);
  digitalWrite(PA9, LOW);
}

void five()
{
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PA9, HIGH);
  digitalWrite(PA10, LOW);
}

void four()
{
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PA10, HIGH);
  digitalWrite(PA11, LOW);
}

void three()
{
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PA11, HIGH);
  digitalWrite(PB4, LOW);
}

void two()
{
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PB4, HIGH);
  digitalWrite(PB5, LOW);
}

void one()
{
  /*-------------------------------------------------------------------------------*/
  digitalWrite(PB5, HIGH);
  digitalWrite(PA8, LOW);
}

void GET_PROFILES()
{

  switch (led)
  {

  case 1:
    /* PROFILE INFO */
    /* PROFILE INFO */
    /* PROFILE INFO */

    Q(4); //1, 3, 4, 2; 1 is stronge
    mode = true;
    profilearray[1] = 200; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 60;  //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 0; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 0; //  HP Profile Variable or number

    profilearray[4] = 4096;              // BP CENTRE VOLUME
    profilearray[5] = lightsag[trigger]; //env_duty; //BP Profile Variable or number

    profilearray[6] = 2000;               // LP CENTRE VOLUME
    profilearray[7] = -lightsag[trigger]; // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 4096; //higher scope of centre frequency knob*4
    profilearray[11] = 0;    //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = env_duty * 2; //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 0;            // top of the audio envelope
    profilearray[12] = 1500;        // BOttom of the audio envelope

    break;

  case 2:
    Q(1); //1, 3, 4, 2; 1 is stronge
    mode = true;
    profilearray[1] = 200; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 60;  //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 2000; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 0;    //  HP Profile Variable or number

    profilearray[4] = 4096;              // BP CENTRE VOLUME
    profilearray[5] = lightsag[trigger]; //env_duty; //BP Profile Variable or number

    profilearray[6] = 2000;               // LP CENTRE VOLUME
    profilearray[7] = -lightsag[trigger]; // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 4096; //higher scope of centre frequency knob*4
    profilearray[11] = 2000; //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = env_duty; //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 4000;     // top of the audio envelope
    profilearray[12] = 0;       // BOttom of the audio envelope

    break;

  case 3:
    mode = true;
    Q(4);                  //1, 3, 4, 2; 1 is stronge
    profilearray[1] = 400; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 50;  //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 2000; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 0;    //compress3[trigger];   //  HP Profile Variable or number

    profilearray[4] = 0; // BP CENTRE VOLUME
    profilearray[5] = 0; //env_duty; //BP Profile Variable or number

    profilearray[6] = 4096; // LP CENTRE VOLUME
    profilearray[7] = 0;    //compress3[trigger];  // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 3000; //higher scope of centre frequency knob*4
    profilearray[11] = 1500; //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = env_duty * 3; //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 2500;         // top of the audio envelope
    profilearray[12] = 0;           // BOttom of the audio envelope

    break;

  case 4:
    /* PROFILE INFO */
    /* PROFILE INFO */
    mode = false;
    Q(4);                  //1, 3, 4, 2; 1 is stronge
    profilearray[1] = 300; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 50;  //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 2000;               //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = compress3[trigger]; //  HP Profile Variable or number

    profilearray[4] = 0; // BP CENTRE VOLUME
    profilearray[5] = 0; //env_duty; //BP Profile Variable or number

    profilearray[6] = 4096;               // LP CENTRE VOLUME
    profilearray[7] = compress3[trigger]; // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 3000; //higher scope of centre frequency knob*4
    profilearray[11] = 1500; //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = env_duty * 2; //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 2500;         // top of the audio envelope
    profilearray[12] = 0;           // BOttom of the audio envelope

    break;

  case 5:
    /* PROFILE INFO */
    Q(4); //1, 3, 4, 2; 1 is stronge
    mode = true;
    profilearray[1] = 500; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 100; //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 3000; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 0;    //  HP Profile Variable or number

    profilearray[4] = 3000; // BP CENTRE VOLUME
    profilearray[5] = comp; //env_duty; //BP Profile Variable or number

    profilearray[6] = 3000; // LP CENTRE VOLUME
    profilearray[7] = 0;    // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 4095; //higher scope of centre frequency knob*4
    profilearray[11] = 500;  //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = 0;    //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 2500; // top of the audio envelope
    profilearray[12] = 0;   // BOttom of the audio envelope

    break;

  case 6:
    /* PROFILE INFO */
    //put into array?
    mode = true;
    profilearray[1] = 500; //ENV LIMITER COMPRESS threshold level - put high for none.
    profilearray[9] = 100; //COMPRESSION ratio as percentage 0 as limit, 100 as full

    profilearray[2] = 2000; //4096-env_duty; //HP CENTRE VOLUME
    profilearray[3] = 0;    //  HP Profile Variable or number

    profilearray[4] = 2000; // BP CENTRE VOLUME
    profilearray[5] = 0;    //env_duty; //BP Profile Variable or number

    profilearray[6] = 2000; // LP CENTRE VOLUME
    profilearray[7] = 0;    // LP Profile Variable or number

    //FREQUENCY KNOB
    profilearray[10] = 4095; //higher scope of centre frequency knob*4
    profilearray[11] = 500;  //lower scope of centre frequency knob

    //FREQUENCY AUDIO
    profilearray[8] = 0;    //BP Profile Variable or number; // Freq Profile, Variable or number
    profilearray[0] = 2500; // top of the audio envelope
    profilearray[12] = 0;   // BOttom of the audio envelope
  }
}
