/*

Run Initialisation:

Riase up the vertical
get a horizontal zero and position in the middle of the enclosure.
get a vertical zero and raise back up
*/
int firsttime = 0;
int dir= 3;
int ena= 2;
int stp = 4;


int dirhorz= 8;
int enahorz= 19;
int stphorz = 7;

int vert_mag = A6;
int horz_mag = A7;
int horz;
int vert; 
int btn = 6;
int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
int ledState = -1; //this variable tracks the state of the LED, negative if off, positive if on
 
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
 
//==========================
void setup() {
// initialize digital pin LED_BUILTIN as an output.
pinMode(stp, OUTPUT);
pinMode(dir, OUTPUT);
pinMode(ena, OUTPUT);
pinMode(13, OUTPUT);
pinMode(stphorz, OUTPUT);
pinMode(dirhorz, OUTPUT);
pinMode(enahorz, OUTPUT);
pinMode(PB1, OUTPUT);

pinMode(vert_mag, INPUT);
pinMode(horz_mag, INPUT);
pinMode(btn, INPUT);
digitalWrite(enahorz, LOW);
digitalWrite(ena, LOW);
digitalWrite(PB1, LOW);


}
//==========================
void loop() {


//sample the state of the button - is it pressed or not?
buttonState = digitalRead(btn);
  //filter out any noise by setting a time buffer
if ( (millis() - lastDebounceTime) > debounceDelay) {
if (buttonState == HIGH){


if(firsttime == 0){
  Initialise();
  }

  
   lastDebounceTime = millis(); //set the current time
}






}
if ( (millis() - lastDebounceTime) > debounceDelay) {
if (buttonState == HIGH){
if(firsttime == 1){
firsttime = 2;
}
}}


if(firsttime == 2){
vert = analogRead(vert_mag);
  Routine();} 
   //
 /* 
  for(int j=0;j<5000;j++){ // height the router goes up
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(100); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(100);

} 

*/
  

  

}

/*make 1500speed with coming inatgreater depth
 * 
 * */
 

void Routine(){

//drop bit down//


digitalWrite(dir, HIGH);
do {

digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
digitalWrite(13, HIGH); 
delayMicroseconds(200); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
digitalWrite(13, LOW); 
delayMicroseconds(200);
vert = analogRead(vert_mag);
} while (vert >= 500);




digitalWrite(dirhorz, LOW);
do {

for(int j=0;j<400;j++){  
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delay(100); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delay(100);
horz = analogRead(horz_mag);
if (horz <= 500){break;}
 }
 for(int j=0;j<1150;j++){  
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(200); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(200);
horz = analogRead(horz_mag);
if (horz <= 500){break;}
 }
for(int j=0;j<350;j++){  
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delay(100); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delay(100);
horz = analogRead(horz_mag);
if (horz <= 500){break;}
 }
} while (horz >= 500);


digitalWrite(dirhorz, HIGH);
 for(int i=0;i<2100;i++){
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delay(10); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delay(10);
}   



  firsttime = 3;
}









void Initialise(){

digitalWrite(dir, HIGH);
do {
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(200); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(200);
vert = analogRead(vert_mag);
} while (vert >= 500);
digitalWrite(dir, LOW);
for(int j=0;j<10000;j++){ // height the router goes up
digitalWrite(stp, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(100); // wait for a second
digitalWrite(stp, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(100);

} 

digitalWrite(dirhorz, LOW);

do {
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(500); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(500);
horz = analogRead(horz_mag);
} while (horz >= 500);
digitalWrite(dirhorz, HIGH);
for(int i=0;i<2100;i++){
digitalWrite(stphorz, HIGH); // turn the LED on (HIGH is the voltage level)
delayMicroseconds(200); // wait for a second
digitalWrite(stphorz, LOW); // turn the LED off by making the voltage LOW
delayMicroseconds(200);
}


firsttime = 1;
}
