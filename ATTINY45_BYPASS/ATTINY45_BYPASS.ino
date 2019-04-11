
//GOLDTOOTH BYPASSCODE



int state = 0;
#define relay 3
#define phototrans 1
#define btn_onoff 4
long debouncing_time = 10000; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
long delaytime = 750;
void setup() {
  
  //
  // put your setup code here, to run once:


pinMode(phototrans, OUTPUT);
pinMode(relay, OUTPUT);
pinMode(btn_onoff,INPUT_PULLUP);
 digitalWrite(phototrans, LOW);
}

void loop() {
if (digitalRead(btn_onoff)==LOW){ONANDOFF();}
}

void ONANDOFF() {
  if((long)(micros() - last_micros) >= debouncing_time) {

 digitalWrite(phototrans, HIGH); 
 delayMicroseconds(delaytime);
 if (state ==0){digitalWrite(relay, HIGH); state = 1;}
 else {digitalWrite(relay, LOW);state = 0;
}

  }
  delayMicroseconds(delaytime);
 digitalWrite(phototrans, LOW);
 last_micros = micros();
}

