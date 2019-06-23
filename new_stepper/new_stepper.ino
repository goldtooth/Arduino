/*
 * NEW ROUTER
 * 
 * 
 * 
 */
int firsttime = 0;
int dir= 10;//8 ///10
int stp= 12; //10   //8
int ena = 8;  //12 //12

int mag = A7;
int btn = A5;
int vert;
int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
 





void setup() {
  // put your setup code here, to run once:
pinMode(stp, OUTPUT);
pinMode(dir, OUTPUT);
pinMode(ena, OUTPUT);
pinMode(13, OUTPUT);
pinMode(mag, INPUT);
pinMode(btn, INPUT);
digitalWrite(btn, LOW);

//Initialise();
//routine();
}

void loop() {
  // put your main code here, to run repeatedly:

//sample the state of the button - is it pressed or not?
buttonState = digitalRead(btn);
  //filter out any noise by setting a time buffer
if (buttonState == HIGH){
if ( (millis() - lastDebounceTime) > debounceDelay) {
if(firsttime == 0){
Initialise();
 }else{
routine();
}

  
lastDebounceTime = millis(); //set the current time
}
}
}




void Initialise(){
  //scroll back until mag sys no more  
digitalWrite(dir, LOW);
do {

digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
digitalWrite(13, HIGH); 
delayMicroseconds(20); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
digitalWrite(13, LOW); 
delayMicroseconds(20);
vert = analogRead(mag);
  } while (vert >= 500);
 firsttime = 1; 
}





void routine(){
// move forward to near the router, stop and move forward very slowly..  
/////////////////////////////////////
digitalWrite(dir, HIGH); // LOW id back towards begining, high is towards router
for(int j=0;j<30000;j++){  
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(100); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(100);
  }
/////////////////////////////////////

digitalWrite(dir, HIGH); // LOW id back towards begining, high is towards router
for(int j=0;j<12000;j++){  
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(3000); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(3000);
  }



/////////////////////////////////////
digitalWrite(dir, HIGH); // LOW id back towards begining, high is towards router
for(int j=0;j<32000;j++){  
digitalWrite(stp, HIGH);// turn the LED on (HIGH is the voltage level)
delayMicroseconds(100); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(100);
  }

for(int j=0;j<5000;j++){  
digitalWrite(stp, HIGH);// turn the LED on (HIGH is the voltage level)
delayMicroseconds(300); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(300);
}
/////////////////////////////////////



digitalWrite(dir, HIGH); // LOW id back towards begining, high is towards router
for(int j=0;j<14000;j++){  
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(3000); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(3000);
  }



  
 firsttime = 0; 
  }
