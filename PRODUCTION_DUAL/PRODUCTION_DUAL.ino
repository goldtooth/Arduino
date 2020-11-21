
#include <ADG715.h>
// The Arduino pin used for the slave select / chip select

#define phototrans 9
#define relay 8
int delaytime = 2000;
#define led1 6
#define led2 3


#define btn_onoff 2
#define btn_wave 7


long debouncing_time = 500; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
bool permission = true;
int iii;
int led;
int b; 
int enact = 1;
int wavetablelength = 512;                                                                                     
int relay_damper_ms = 10;
int breaker = 1;
int rate;
int state = 0;
int hate = 0;
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
int buttonState = 0; 
int buttonState1 = 0; 
ADG715 adg715(0x48);


void setup() {

  // Set the SPI frequency to 1 MHz (on 16 MHz Arduinos), to be safe.
  // DIV2 = 8 MHz works for me, though, even on a breadboard.
  // This is not strictly required, as there is a default setting.

  // Use "port writes", see the manual page. In short, if you use pin 10 for
  // SS (and pin 7 for LDAC, if used), this is much faster.
  // Also not strictly required (no setup() code is needed at all).
 
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


  pinMode(relay, OUTPUT); //relay
  pinMode(phototrans, OUTPUT); //relay


      // disable all interrupts
  // Setup the second button with an internal pull-up :
pinMode(btn_onoff, INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(btn_onoff), ONANDOFF, FALLING);
pinMode(btn_wave, INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(btn_wave), debounce, LOW); //FALLING? OR RISING? as a way to not have theinterupt triggering when depressed for a long time
  
CHANGEWAVE();
digitalWrite(phototrans, LOW);
  //sine()d
}


void loop() {
buttonState = digitalRead(btn_wave);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
if ((unsigned long)(micros() - last_micros) >= (debouncing_time * 500) && permission) {
permission=false;

      CHANGEWAVE();  // turn LED on:
      last_micros = micros();

  } else {
    // turn LED off:

  }
  }

buttonState1 = digitalRead(btn_onoff);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState1 == LOW) {
if ((unsigned long)(micros() - last_micros) >= (debouncing_time * 500) && permission) {
permission=false;


TOGGLE_HW_RELAY();


last_micros = micros();

}




      
  } else {
    // turn LED off:

  }
  

if (buttonState1 == HIGH) {
if ((unsigned long)(micros() - last_micros) >= (debouncing_time * 1500) && !permission) {  
permission=true;
}
}

if (buttonState == HIGH) {
if ((unsigned long)(micros() - last_micros) >= (debouncing_time * 1500) && !permission) {  
permission=true;
}
}


}




void TOGGLE_HW_RELAY(){
digitalWrite(phototrans, HIGH);
delay(25);
if (state == 0) {
      PORTB |= _BV(PB0); //8
      state = 1;
    }
    else {
      PORTB &= ~_BV(PB0); //8
      state = 0;
    }

delay(25);
digitalWrite(phototrans, LOW);
   
 } 




 
  
void CHANGEWAVE() {
  adg715.begin();
digitalWrite(phototrans, HIGH);
delayMicroseconds(delaytime);
    led++;
    if (led > 3) {
      led = 1;
    }
   
  
  switch (led) {
    case 1:
      FlipIt();
      break;

    case 2:
      ReverseIt();
      break;

          case 3:
      StallIt();
      break;

          case 4:
      SellIt();
      break;
}
delayMicroseconds(delaytime);
digitalWrite(phototrans, LOW);
}

void FlipIt() {
digitalWrite(led2, LOW);
digitalWrite(led1, HIGH);
delay(100);
adg715.writeChannel(1,255);
adg715.writeChannel(5,255);
adg715.writeChannel(6,255);
adg715.writeChannel(2,0);
adg715.writeChannel(3,0);
adg715.writeChannel(4,0);

adg715.writeChannel(8,0);

}

void ReverseIt() {
digitalWrite(led1, LOW);
digitalWrite(led2, HIGH);
delay(100);
adg715.writeChannel(1,0);
adg715.writeChannel(5,0);
adg715.writeChannel(6,0);
adg715.writeChannel(2,255);
adg715.writeChannel(3,255);
adg715.writeChannel(4,255);

adg715.writeChannel(8,0);

//-
}

void StallIt(){
digitalWrite(led1, HIGH);
digitalWrite(led2, HIGH);
delay(100);
adg715.writeChannel(1,255);
adg715.writeChannel(2,255);
adg715.writeChannel(3,0);
adg715.writeChannel(4,255);
adg715.writeChannel(5,0);
adg715.writeChannel(6,255);

adg715.writeChannel(8,0); //compensate volume.
}

void SellIt(){
digitalWrite(led1, LOW);
digitalWrite(led2, LOW);
delay(100);
adg715.writeChannel(1,255);
adg715.writeChannel(2,255);
adg715.writeChannel(3,0);
adg715.writeChannel(4,255);
adg715.writeChannel(5,0);
adg715.writeChannel(6,255);

adg715.writeChannel(8,255);
}
