#include <ShiftRegister74HC595.h> //https://github.com/Simsso/ShiftRegister74HC595
#include <MCP45HVX1.h> //https://github.com/WifWaf/MCP45HVX1
#include <EEPROM.h>


#define STATE_NORMAL 0 //static const int  STATE_NORMAL = 0; // no button activity
#define STATE_SHORT 1 //static const int  STATE_SHORT  = 1; // short button press
#define STATE_LONG 2 //static const int  STATE_LONG  = 2; // long button press
#define STATE_OPTIONS 4
#define EXIT_OPTIONS 3
#define STATE_NORMAL_OPTIONS 5 //static const int  STATE_NORMAL = 0; // no button activity
#define STATE_SHORT_OPTIONS 6 //static const int  STATE_SHORT  = 1; // short button press
#define STATE_LONG_OPTIONS 7 //static const int  STATE_LONG  = 2; // long button press


volatile int  resultButton = 0; // global value set by checkButton()

//AD444 SWITCH - HIGH.LOW DIGITAL CONTROL - DONE
//MCP45HVX1 - DIGITALPOT - MCP45HVX1 - https://github.com/WifWaf/MCP45HVX1
//74HC595 - Arduino Libs

#define feedforward A0
#define feedback A1
#define smooth A2
#define bassresponse A3

#define phototrans 9
#define relay 8
int delaytime = 1000;
int led3;


#define btn_onoff 3
#define btn_wave 2

long debouncing_time = 250; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
int mode=1;
int iii;
int led;
int led2;
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
int buttonState = HIGH; 
int lastButtonState;
boolean dont = false; 
boolean inoptions = false; 
boolean smooth_cap = false;
boolean bassresponse_cap = false;


ShiftRegister74HC595<1> sr(5, 0, 1); //sr.set(i, HIGH);
MCP45HVX1 digiPot(0x3C); //check address is correct
//digiPot.writeWiper(200);

void setup() {
  // parameters: <number of shift registers> (data pin, clock pin, latch pin)

  digiPot.begin();  
  // put your setup code here, to run once:




  pinMode(feedforward, OUTPUT);
  pinMode(feedback, OUTPUT);
  pinMode(smooth, OUTPUT);
  pinMode(bassresponse, OUTPUT);
  
  pinMode(relay, OUTPUT); //relay
  pinMode(phototrans, OUTPUT); //relay


 // disable all interrupts
  // Setup the second button with an internal pull-up :
pinMode(btn_onoff, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(btn_onoff), ONANDOFF, FALLING);
pinMode(btn_wave, INPUT);
//attachInterrupt(digitalPinToInterrupt(btn_wave), checkButton, RISING); // or rising / or change CHANGEPRESET


STARTUP_FUNCTION();
//led = EEPROM.read(1);

digitalWrite(phototrans, LOW);
  //sine()d
}

void STARTUP_FUNCTION(){
smooth_cap = EEPROM.read(2);
bassresponse_cap = EEPROM.read(3);
delay(10);
funct_smooth();
delay(10);
funct_bassresponse();
delay(10);
funct_smooth();
delay(10);
funct_bassresponse();
delay(10);
led2 = EEPROM.read(1);
switch (led2) {
    case 1:
      funct_feedforward();
      break;

    case 2:
      funct_feedback();
      break;

          case 3:
      funct_parra();
      break;
delay(100);

 
}

    sr.set(0, HIGH);sr.set(4, HIGH);
    delay(10);
    sr.setAllLow(); // set all pins LOW
    delay(10);
    sr.set(1, HIGH);sr.set(5, HIGH);
    delay(10);
    sr.setAllLow(); // set all pins LOW
    delay(30);
  


led = EEPROM.read(4);
switch (led) {
    case 1:
      Ratio1();
      break;

    case 2:
      Ratio2();
      break;

          case 3:
      Ratio3();
      break;

          case 4:
      Ratio4();
      break;
}

}




void loop() {
buttonState = digitalRead(btn_wave);
if (buttonState != lastButtonState) {
checkButton();
 lastButtonState = buttonState;
  }

  

 

  switch (resultButton) {
  
  case STATE_NORMAL: {
  break;
  }



  case STATE_SHORT: {
 ADD_AND_CHANGE_RATIO();
  resultButton=STATE_NORMAL;
  break;
  }


  
  case STATE_LONG: {
  CHANGEFUNCTION();
  CHANGE_RATIO();
resultButton=STATE_NORMAL;
  break;
  }


  case STATE_NORMAL_OPTIONS: {
  break;
  }

 case STATE_SHORT_OPTIONS: {

funct_bassresponse();
 resultButton=STATE_NORMAL;
 
  break;
  }

  case STATE_LONG_OPTIONS: {

funct_smooth();
 resultButton=STATE_NORMAL;
    
  break;
  }


  }

if(inoptions){
  if ( (millis() % 10) == 0) { 
sr.set(0, HIGH);
sr.set(4, HIGH);
sr.set(1, HIGH);
sr.set(5, HIGH); 
}else{
sr.set(0, LOW);
sr.set(4, LOW);
sr.set(1, LOW);
sr.set(5, LOW); 
}


}
  
}


void ONANDOFF() {

 if(((long)(micros() - last_micros) >= debouncing_time * 50) && (digitalRead(btn_wave)==LOW)) {
TOGGLE_HW_RELAY();
last_micros = micros();
}
else if(((long)(micros() - last_micros) >= debouncing_time * 50) && (digitalRead(btn_wave)==HIGH)){
last_micros = micros();

if(!inoptions){ 
sr.setAllLow(); 
dont = true; 
OPTIONS();
readstates();
}else{
sr.setAllLow(); // set all pins LOW
dont = true;
EXITDANCE();
}
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



void debounce(){
 if ((long)(micros() - last_micros2) >= debouncing_time * 150) {
CHANGE_RATIO();
last_micros2 = micros();
} 
}
 
  void ADD_AND_CHANGE_RATIO(){
  led++;
    if (led > 4) {
      led = 1;
    }
    CHANGE_RATIO();
    
  }


void EXITDANCE(){
inoptions=false;
dont = true;
resultButton = STATE_NORMAL | resultButton;
}



void OPTIONS(){
inoptions=true;
resultButton = STATE_NORMAL | resultButton;
 } 




void CHANGE_RATIO() {

  
  switch (led) {
    case 1:
      Ratio1();
      break;

    case 2:
      Ratio2();
      break;

          case 3:
      Ratio3();
      break;

          case 4:
      Ratio4();
      break;
}
EEPROM.write(4,led);
}

void Ratio1() {
  sr.setAllLow(); // set all pins LOW 
 
digiPot.writeWiper(0);
if(mode==1){sr.set(0, HIGH);}else if(mode==2){sr.set(1, HIGH);}else{sr.set(1, HIGH);sr.set(0, HIGH);}
}

void Ratio2() {
    sr.setAllLow(); // set all pins LOW
   
digiPot.writeWiper(50);
if(mode==1){sr.set(2, HIGH);}else if(mode==2){sr.set(3, HIGH);}else{sr.set(2, HIGH);sr.set(3, HIGH);}

//-
}

void Ratio3(){
sr.setAllLow(); // set all pins LOW
digiPot.writeWiper(127);
if(mode==1){sr.set(6, HIGH);}else if(mode==2){sr.set(7, HIGH);}else{sr.set(6, HIGH);sr.set(7, HIGH);}
}

void Ratio4(){
    sr.setAllLow(); // set all pins LOW

digiPot.writeWiper(254);

if(mode==1){sr.set(4, HIGH);}else if(mode==2){sr.set(5, HIGH);}else{sr.set(4, HIGH);sr.set(5, HIGH);}

//uint8_t pinValues[] = { B10101010 };
//sr.setAll(pinValues);

}




void CHANGEFUNCTION() {


    led2++;
    if (led2 > 3) {
      led2 = 1;
    }
   
  
  switch (led2) {
    case 1:
      funct_feedforward();
      break;

    case 2:
      funct_feedback();
      break;

          case 3:
      funct_parra();
      break;

 
}
EEPROM.write(1,led2);

}

void funct_feedforward() {
digitalWrite(feedforward,LOW);
digitalWrite(feedback,HIGH);
mode = 1;

}

void funct_feedback() {
digitalWrite(feedback,LOW);
digitalWrite(feedforward,HIGH);
mode = 2;
}

void funct_parra(){
digitalWrite(feedback,LOW);
digitalWrite(feedforward,LOW);
mode = 3;

}

void funct_smooth(){
 if(smooth_cap){
 smooth_cap=false; 
 digitalWrite(smooth,HIGH);
 sr.set(2, HIGH);
 sr.set(3, LOW);
 }
 else
 {
   smooth_cap=true; 
   digitalWrite(smooth,LOW);
   sr.set(2, LOW);
   sr.set(3, HIGH);
   }
  EEPROM.write(2, smooth_cap); 
  }

 void funct_bassresponse(){
 if(bassresponse_cap){
 bassresponse_cap=false;
 digitalWrite(bassresponse,HIGH);
 sr.set(6, HIGH);
 sr.set(7, LOW);
 }
 else{ 
   bassresponse_cap=true; 
   digitalWrite(bassresponse,LOW);
   sr.set(6, LOW);
   sr.set(7, HIGH);
   }

EEPROM.write(3, bassresponse_cap); 
  }
  void readstates(){
 if(bassresponse_cap){  sr.set(6, LOW);sr.set(7, HIGH);}else{ sr.set(6, HIGH);sr.set(7, LOW);}
 if(smooth_cap){  sr.set(2, LOW);sr.set(3, HIGH);}else{ sr.set(2, HIGH);sr.set(3, LOW);}
  }


//--Routine to create two levels of Button Press
void checkButton() {
  /*
  * This function implements software debouncing for a two-state button.
  * It responds to a short press and a long press and identifies between
  * the two states. Your sketch can continue processing while the button
  * function is driven by pin changes.
  */

  const unsigned long LONG_DELTA = 30000;               // hold seconds for a long press
  const unsigned long DEBOUNCE_DELTA = 3000;        // debounce time
  static int lastButtonStatus = LOW;                                   // HIGH indicates the button is NOT pressed
  int buttonStatus;                                                                    // button atate Pressed/LOW; Open/HIGH
  static unsigned long longTime = 0, shortTime = 0; // future times to determine is button has been poressed a short or long time
  boolean Released = true, Transition = false;                  // various button states
  boolean DOWNPRESSED = true;
  boolean latcher = true;
  boolean timeoutShort = false, timeoutLong = false;    // flags for the state of the presses

  buttonStatus = digitalRead(btn_wave);                // read the button state on the pin "BUTTON_PIN"
  timeoutShort = (micros() > shortTime);                          // calculate the current time states for the button presses
  timeoutLong = (micros() > longTime);

                // reset the timeouts if the button state changed
     
     if (buttonStatus != lastButtonStatus) { 
      shortTime = micros() + DEBOUNCE_DELTA;
      longTime = micros() + LONG_DELTA;
     }
  

 
  Transition = (buttonStatus != lastButtonStatus);        // has the button changed state
  Released = (Transition && (buttonStatus == LOW)); // for input pullup circuit
  lastButtonStatus = buttonStatus;                                     // save the button status

  if ( ! Transition) {                                                                //without a transition, there's no change in input
  // if there has not been a transition, don't change the previous result
       resultButton =  STATE_NORMAL | resultButton;
       return;
  }



  
  
  if (timeoutLong && (buttonStatus == LOW) && (inoptions == false) && (dont==false) ){
    CHANGEFUNCTION();
    CHANGE_RATIO();
    // long timeout has occurred and the button was just released
       resultButton = STATE_LONG | resultButton;       // ensure the button result reflects a long press
  } else if (timeoutShort && Released && (inoptions == false)) {                          // short timeout has occurred (and not long timeout) and button was just released
      resultButton = STATE_SHORT | resultButton;     // ensure the button result reflects a short press
  } else if ((inoptions == false)&& (dont==false) ){                                                                                  // else there is no change in status, return the normal state
      resultButton = STATE_NORMAL | resultButton; // with no change in status, ensure no change in button status
  } else if (timeoutLong && Released && (inoptions == true) && (dont==false)) {
    // long timeout has occurred and the button was just released
       resultButton = STATE_LONG_OPTIONS | resultButton;       // ensure the button result reflects a long press
  } else if (timeoutShort && Released && (inoptions == true) && (dont==false)) {                          // short timeout has occurred (and not long timeout) and button was just released
      resultButton = STATE_SHORT_OPTIONS | resultButton;     // ensure the button result reflects a short press
  } else{dont=false;}





}

  
