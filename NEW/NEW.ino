#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>

HardwareTimer timer(2);
#define TIMER_RATE 10000

const uint16_t profile1[512] =
{
22,0,0,11,14,16,19,22,
29,37,44,48,52,56,60,64,
68,73,77,81,85,90,94,99,
103,107,111,115,119,123,127,131,
137,142,148,153,159,164,170,175,
180,185,189,194,199,203,208,212,
217,221,226,230,235,239,244,249,
253,258,263,267,272,276,280,285,
290,296,301,307,311,315,320,324,
329,336,343,350,354,358,361,365,
369,372,376,380,383,391,398,405,
408,411,413,416,422,427,431,434,
438,444,449,455,460,464,467,471,
478,486,493,498,504,507,511,515,
520,526,531,537,542,548,550,553,
556,559,564,569,577,584,591,595,
599,602,610,617,624,635,646,654,
663,671,679,683,686,690,701,712,
723,726,730,734,745,756,764,772,
780,789,799,810,814,818,821,832,
843,849,854,865,876,887,898,909,
920,931,936,942,953,964,969,975,
986,997,1008,1013,1019,1026,1033,1040,
1051,1062,1070,1077,1084,1092,1099,1106,
1117,1128,1139,1147,1155,1164,1172,1179,
1186,1194,1202,1210,1218,1227,1235,1243,
1251,1259,1267,1274,1281,1290,1298,1306,
1314,1322,1329,1336,1343,1351,1358,1369,
1380,1391,1402,1409,1416,1424,1435,1446,
1457,1468,1479,1489,1500,1511,1522,1533,
1544,1555,1566,1577,1583,1588,1593,1599,
1610,1621,1632,1643,1654,1665,1672,1679,
1687,1692,1698,1705,1712,1719,1730,1741,
1749,1756,1763,1769,1774,1780,1785,1792,
1800,1807,1811,1814,1818,1829,1840,1845,
1851,1856,1862,1867,1873,1884,1889,1895,
1906,1911,1917,1928,1938,1944,1949,1960,
1966,1971,1971,1982,1982,1982,1982,1993,
1993,1993,1993,1993,2004,2004,2004,2015,
2015,2015,2015,2015,2015,2015,2015,2015,
2015,2015,2015,2015,2015,2015,2015,2015,
2015,2015,2015,2015,2015,2015,2015,2015,
2015,2015,2015,2015,2015,2015,2015,2015,
2015,2015,2015,2015,2021,2026,2026,2026,
2026,2026,2026,2026,2037,2037,2037,2037,
2037,2037,2037,2048,2048,2048,2048,2048,
2048,2048,2043,2037,2030,2022,2015,2015,
2015,2015,2004,2004,2004,2004,2004,2004,
2004,2004,2004,2004,2004,2001,1997,1993,
1993,1993,1993,1993,1993,1993,1993,1993,
1993,1993,1993,1993,1993,1993,1993,1993,
1993,1993,1993,1993,1993,1982,1971,1960,
1960,1949,1949,1938,1938,1928,1928,1917,
1917,1906,1902,1898,1895,1891,1887,1884,
1878,1873,1867,1862,1856,1851,1847,1844,
1840,1829,1829,1818,1807,1807,1796,1791,
1785,1780,1763,1752,1752,1741,1730,1719,
1698,1687,1676,1676,1654,1643,1632,1610,
1599,1588,1566,1566,1555,1544,1533,1522,
1515,1508,1500,1479,1475,1471,1468,1462,
1457,1446,1435,1431,1427,1424,1418,1413,
1413,1413,1407,1402,1398,1395,1391,1391,
1391,1380,1380,1380,1380,1380,1380,1380,
1380,1380,1380,1391,1391,1391,1391,498};


const uint16_t profile2[512] =
{

2250,2258,2266,2274,2282,2290,2298,2306,
2314,2322,2330,2338,2346,2354,2362,2370,
2378,2386,2394,2402,2410,2418,2426,2434,
2442,2450,2458,2466,2474,2482,2490,2498,
2506,2514,2522,2530,2538,2546,2554,2562,
2570,2578,2586,2594,2602,2610,2618,2626,
2634,2642,2650,2658,2666,2674,2682,2690,
2698,2706,2714,2722,2730,2738,2746,2754,
2762,2770,2778,2786,2794,2802,2810,2818,
2826,2834,2842,2850,2858,2866,2874,2882,
2890,2898,2906,2914,2922,2930,2938,2946,
2954,2962,2970,2978,2986,2994,3002,3010,
3018,3026,3034,3042,3050,3058,3066,3074,
8,16,24,32,40,48,56,64,
72,80,88,96,104,112,120,128,
136,144,152,160,168,176,184,192,
200,208,216,224,232,240,248,256,
264,272,280,288,296,304,312,320,
328,336,344,352,360,368,376,384,
392,400,408,416,424,432,440,448,
456,464,472,480,488,496,504,512,
520,528,536,544,552,560,568,576,
584,592,600,608,616,624,632,640,
648,656,664,672,680,689,697,705,
713,721,729,737,745,753,761,769,
777,785,793,801,809,817,825,833,
841,849,857,865,873,881,889,897,
905,913,921,929,937,945,953,961,
969,977,985,993,1001,1009,1017,1025,
1033,1041,1049,1057,1065,1073,1081,1089,
1097,1105,1113,1121,1129,1137,1145,1153,
1161,1169,1177,1185,1193,1201,1209,1217,
1225,1233,1241,1249,1257,1265,1273,1281,
1289,1297,1305,1313,1321,1329,1337,1345,
1353,1361,1369,1377,1385,1393,1401,1409,
1417,1425,1433,1441,1449,1457,1465,1473,
1481,1489,1497,1505,1513,1521,1529,1537,
1545,1553,1561,1569,1577,1585,1593,1601,
1609,1617,1625,1633,1641,1649,1657,1665,
1673,1681,1689,1697,1705,1713,1721,1729,
1737,1745,1753,1761,1769,1777,1785,1793,
3723,3731,3739,3747,3755,3763,3771,3779,
3787,3795,3803,3811,3819,3827,3835,3843,
3851,3859,3867,3875,3883,3891,3899,3907,
3915,3923,3931,3939,3947,3955,3963,3971,
3979,3987,3995,4003,4011,4019,4027,4035,
4043,4051,4059,4067,4075,4083,4091,4095,
1801,1809,1817,1825,1833,1841,1849,1857,
1865,1873,1881,1889,1897,1905,1913,1921,
1929,1937,1945,1953,1961,1969,1977,1985,
1993,2001,2009,2017,2025,2033,2041,2050,
2058,2066,2074,2082,2090,2098,2106,2114,
2122,2130,2138,2146,2154,2162,2170,2178,
2186,2194,2202,2210,2218,2226,2234,2242,
3082,3090,3098,3106,3114,3122,3130,3138,
3146,3154,3162,3170,3178,3186,3194,3202,
3210,3218,3226,3234,3242,3250,3258,3266,
3274,3282,3290,3298,3306,3314,3322,3330,
3338,3346,3354,3362,3370,3378,3386,3394,
3402,3410,3419,3427,3435,3443,3451,3459,
3467,3475,3483,3491,3499,3507,3515,3523,
3531,3539,3547,3555,3563,3571,3579,3587,
3595,3603,3611,3619,3627,3635,3643,3651,
3659,3667,3675,3683,3691,3699,3707,3715,
   
};

const uint16_t profile3[512] = { 
722,716,716,716,716,716,716,716,
716,716,716,728,962,1014,1065,1128,
1193,1264,1294,1325,1355,1398,1441,1462,
1483,1503,1521,1539,1557,1583,1610,1631,
1651,1667,1683,1699,1736,1773,1801,1829,
1844,1859,1873,1891,1909,1925,1941,1965,
1975,2001,2015,2036,2039,2045,2047,2048,
2048,2048,2048,2048,2048,2047,2046,2045,
2045,2045,2045,2045,2044,2042,2041,2039,
2038,2037,2036,2035,2033,2033,2030,2027,
2024,2020,2015,2012,2008,2004,1998,698,
657,636,626,616,608,601,592,583,
576,568,558,548,542,536,530,522,
514,506,499,491,484,476,469,462,
456,450,441,432,423,414,405,398,
391,387,383,379,367,361,355,346,
337,332,328,323,318,313,308,303,
299,294,290,286,282,278,273,269,
265,261,257,254,250,246,242,238,
235,231,226,222,218,213,209,204,
198,195,189,186,181,178,175,172,
169,163,160,157,154,149,145,136,
133,130,121,117,112,110,107,104,
100,96,92,87,83,81,78,76,
74,68,67,65,64,62,59,58,
56,55,54,53,50,49,48,47,
47,47,47,46,45,44,44,44,
43,41,39,37,36,36,36,35,
34,33,30,29,28,27,25,24,
22,21,20,19,18,16,14,12,
11,10,9,8,7,6,5,5,
4,4,3,2,1,1,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
1,1,2,3,4,4,5,6,
7,8,9,11,12,13,15,16,
18,19,21,22,24,24,25,26,
27,27,28,29,30,30,30,30,
32,34,36,37,38,41,43,45,
47,50,53,54,56,57,58,59,
61,62,62,62,64,65,65,67,
68,69,70,71,73,74,74,74,
75,77,77,77,78,80,80,80,
80,81,83,86,89,90,91,92,
93,95,96,98,101,104,106,108,
110,112,114,116,118,121,123,125,
128,130,132,134,136,138,141,143,
145,148,151,154,157,160,163,166,
169,172,176,181,186,192,197,201,
207,213,218,222,234,237,249,260,
272,284,290,296,299,302,320,323,
326,334,340,349,355,361,367,370,
374,379,385,391,394,397,400,401,
402,405,408,410,411,414,417,419,
420,423,426,428,429,435,441,442,
444,447,453,459,465,468,471,476,
485,488,494,500,506,509,512,515,
518,524,525,527,533,536,539,542,
548,556,559,568,580,586,666,675,
684,687,701,707,713,716,719,719};

////do not define pins within global!!!!!poo on the shoe
//-->DECLARE  THINGS
long debouncing_time = 150; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;
volatile unsigned long last_micros2;
int poop = 1;
int led = 1;
int i;
int b;
int p;
int a;
//uint16_t h;
int breaker = 1;
int rate;
int state = 0;
int val;
int actdelay;
int dong;
int wrong;
int Adel;
int deep;
int profilearray[12];
bool trigger = false;
bool flip = false;
int shift;
int timer1_counter;
int rel;
int attack;
float depth;
int centre;
float sens;
int hate;
int Threshold = 500;
float AUDIN;
float THELISTENER;
float LARGER;
int RAWIN;
int midmark = 2096;
int offset = 0; //adjust to allow for bias of audio input 
int maxVal = 0;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
float env_sens;
float env;
//-->DECLARE  THINGS

const int RELEASE_NUMSAMPLE = 100; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int RELEASE_SAMP[RELEASE_NUMSAMPLE] = {0}; //value of sample
int RELEASE_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int RELEASE_TOTAL = 0; //running total of sample values

const int LARGER_NUMSAMPLE = 100; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int LARGER_SAMP[LARGER_NUMSAMPLE] = {0}; //value of sample
int LARGER_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int LARGER_TOTAL = 0; //running total of sample values


const int LISTEN_NUMSAMPLE = 100; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int LISTEN_SAMP[LISTEN_NUMSAMPLE] = {0}; //value of sample
int LISTEN_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int LISTEN_TOTAL = 0; //running total of sample values


const int ENVA_NUMSAMPLE = 200; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int ENVA_SAMP[ENVA_NUMSAMPLE] = {0}; //value of sample
int ENVA_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int ENVA_TOTAL = 0; //running total of sample values

float ENV_REL;
float ENV_ATT;
float envattack;
float env_depth = 1.0;
int listener = 10;
int avg = 40;//was 50
int larger_avg = 100;
bool env_up = false;
int HP_VOL_PROFILE ;
int BP_VOL_PROFILE ;
int LP_VOL_PROFILE ; 
int FREQ_PROFILE ;
int MASTER_PROFILE;

int selection[5] = {0,0,0,0,0}; 
int Master;
void setup() {
disableDebugPorts();
dac.begin();  // initialize i2c interface
dac.vdd(3300); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
dac.setVref(0,0,0,0);
//dac.setVref(1,1,1,1);

// -->SET UP PIN MODES

  pinMode(PA8, OUTPUT); //LED
  pinMode(PA9, OUTPUT); //LED
  pinMode(PA10, OUTPUT); //LED
  pinMode(PA11, OUTPUT); //LED
  pinMode(PB4, OUTPUT); //LED
  pinMode(PB5, OUTPUT); //LED
  pinMode(PB8, OUTPUT);//q
  pinMode(PB9, OUTPUT);//q
  pinMode(PA12, OUTPUT); //SS_relay
  pinMode(PA15, OUTPUT); //relay
  pinMode(PA0, INPUT_ANALOG);
  pinMode(PA1, INPUT);//POTS
  pinMode(PA2, INPUT);//POTS
  pinMode(PA3, INPUT);//POTS
  pinMode(PA4, INPUT);//POTS
  pinMode(PA5, INPUT);//POTS
  // Setup the second button with an internal pull-up :
  pinMode(PB13, INPUT_PULLUP);
  attachInterrupt(PB13, CHANGEPRESET, FALLING); // or rising / or change
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, ONANDOFF, FALLING); // or rising / or change
// -->END SET UP PIN MODES

//setupstm timer
timer.pause();
timer.setPeriod(TIMER_RATE); // in microseconds
  // Set up an interrupt on channel 1
timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
timer.attachCompare1Interrupt(handler);
timer.refresh();
timer.resume();

digitalWrite(PB8,LOW);
digitalWrite(PB9,LOW);
digitalWrite(PA8,LOW);
digitalWrite(PA9,LOW);
digitalWrite(PA10,LOW);
digitalWrite(PA11,LOW);
digitalWrite(PB4,LOW);
digitalWrite(PB5,LOW);



  //Setup for the smoother - FILL THE ARRAYS WITH DUMMY VALUES
  for (int RELEASE_READ = 0; RELEASE_READ < RELEASE_NUMSAMPLE; RELEASE_READ++){ //initalizes readings
    RELEASE_SAMP[RELEASE_READ] = 0; //sets readings to 0
  }
  for (int LISTEN_READ = 0; LISTEN_READ < LISTEN_NUMSAMPLE; LISTEN_READ++){ //initalizes readings
    LISTEN_SAMP[LISTEN_READ] = 0; //sets readings to 0
  }
 for (int LARGER_READ = 0; LARGER_READ < LARGER_NUMSAMPLE; LARGER_READ++){ //initalizes readings
    LARGER_SAMP[LARGER_READ] = 0; //sets readings to 0
  }

one();
}

void loop() {
/* */



//actdelay = attack;


// perhaps have env snoorg and triger speed




/* */
//if (i >= 511) {i = 512; trigger = false; }})

/* 
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 2048; //HP CENTRE VOLUME
profilearray[3]  = profile1[i]; //  HP Profile Variable or number

profilearray[4]  = 2048; // BP CENTRE VOLUME
profilearray[5]  = 4095; //BP Profile Variable or number

profilearray[6]  = 2048; // LP CENTRE VOLUME
profilearray[7]  = 0; // LP Profile Variable or number

profilearray[8]  = env; // Freq Profile, Variable or number
profilearray[9]  = 1; //Profile of Master

profilearray[10]  = 3000; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
//selection[] = {HP_VOL_PROFILE, BP_VOL_PROFILE, LP_VOL_PROFILE, FREQ_PROFILE, MASTER_PROFILE}

*/

i++;
if (i > 510) {i = 511;}
if (trigger == true){i = 0; }
if (i < 100) {actdelay = attack;}else{actdelay = rel;}
env = (ENVA_TOTAL / envattack);
env = env  ;

GET_PROFILES();


if (profilearray[0] == 1){env = 0-env;}



OUT1 = profilearray[2] + (profilearray[3]*depth); //HP
OUT2 = profilearray[4] + (profilearray[5]*depth);
OUT3 = profilearray[6] + (profilearray[7]*depth);
OUT4 = centre + (profilearray[8]*depth);
Master = (profilearray[9]*depth);






if (OUT1>4095){OUT1 =4096;}if (OUT1<1){OUT1 =0;}
if (OUT2>4095){OUT2 =4096;}if (OUT2<1){OUT2 =0;}
if (OUT3>4095){OUT3 =4096;}if (OUT3<1){OUT3 =0;}
if (OUT4>4095){OUT4 =4096;}if (OUT4<1){OUT4 =0;}

delayMicroseconds(actdelay);
dac.analogWrite(OUT1,OUT2,OUT3,OUT4);
//--------------- HP, MID, LP, FREQ
//-------------4095 is off,  0 is full



}


void handler(void) {
 checkthings();
}; 





void checkthings(){

/*RELEASE */
rel = analogRead(PA2); // change to relase / smoothing
//envrel = map(rel, 0, 4095, 500, 2000); //made it static at 2000
rel = map(rel, 0, 4095, 0, 9000);

/*ATTACK */
attack = analogRead(PA4); // attack
envattack = map(attack, 0, 4095, 10, 70);
attack = map(attack, 0, 4095, 0, 4000);




/*DEPTH */
depth = analogRead(PA3); // depth
depth = map(depth, 0, 4095, 0, 100);
depth = depth/100;

/*SENSITIVITY */
sens = analogRead(PA1); // chanege to sens
env_sens = map(sens, 0, 4095, 200, 5);
env_sens = env_sens/100;
sens = map(sens, 0, 4095, 500, 5);

/*CENTRE FREQUENCY */
centre = analogRead(PA5); //centre freq    
centre = map(centre, 0, 4095, profilearray[10], profilearray[11]);
/*AUDIO IN */
RAWIN = analogRead(PA0); // AUDIOI
 
 
//Here is the start of the smoother
  RELEASE_TOTAL -= RELEASE_SAMP[RELEASE_INDEX]; //Subtracts the last sample
  RELEASE_SAMP[RELEASE_INDEX] = RAWIN; //sets the sample to the analoge reading (0-1024)
  RELEASE_TOTAL += RELEASE_SAMP[RELEASE_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (RELEASE_INDEX >= avg){ //checks if limit of samples is reached
    RELEASE_INDEX = 0; //resets INDEX to 0, restarting the process
  }


//Here is the start of the smoother
  LISTEN_TOTAL -=  LISTEN_SAMP[LISTEN_INDEX]; //Subtracts the last sample
  LISTEN_SAMP[LISTEN_INDEX] = RAWIN; //sets the sample to the analoge reading (0-1024)
  LISTEN_TOTAL += LISTEN_SAMP[LISTEN_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (LISTEN_INDEX >= listener){ //checks if limit of samples is reached
    LISTEN_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  //Here is the start of the smoother
  
  
  LARGER_TOTAL -=  LARGER_SAMP[LISTEN_INDEX]; //Subtracts the last sample
 LARGER_SAMP[LARGER_INDEX] = RAWIN; //sets the sample to the analoge reading (0-1024)
  LARGER_TOTAL += LARGER_SAMP[LARGER_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (LARGER_INDEX >= larger_avg){ //checks if limit of samples is reached
    LARGER_INDEX = 0; //resets INDEX to 0, restarting the process
  }



  LARGER = LARGER_TOTAL / larger_avg; //slow, is the long average
  THELISTENER = LISTEN_TOTAL / listener; //jumpy, listens for chanes
  AUDIN = RELEASE_TOTAL / avg; // is the mid average


////need to trigger once and reset
if(THELISTENER > (AUDIN + sens) && (THELISTENER > (LARGER *1.2)) && (THELISTENER>500)){
digitalWrite(PA9, HIGH);
trigger = true;
}else{
  digitalWrite(PA9, LOW);
trigger = false;
}


/* 
envelope works as two seperate smoothers - one for attack, one for release,
once triggered go into attack mode, once highest point and decline, then go into release mode.


*/
ENVA_TOTAL -=  ENVA_SAMP[ENVA_INDEX]; //Subtracts the last sample
ENVA_SAMP[ENVA_INDEX] = RAWIN; //sets the sample to the analoge reading (0-1024)
ENVA_TOTAL += ENVA_SAMP[ENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (ENVA_INDEX >= envattack){ //checks if limit of samples is reached
    ENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
  








if (THELISTENER > 500){
  digitalWrite(PB5, HIGH);
}else{digitalWrite(PB5, LOW);}

if (THELISTENER > 1000){
  digitalWrite(PB4, HIGH);
}else{digitalWrite(PB4, LOW);}


if (THELISTENER > 1500){
  digitalWrite(PA11, HIGH);
}else{digitalWrite(PA11, LOW);}


if (THELISTENER > 2000){
  digitalWrite(PA10, HIGH);
}else{digitalWrite(PA10, LOW);}







}















///ALL OTHER BORING STUFF

void ONANDOFF() {
if ((long)(micros() - last_micros) >= debouncing_time * 1500) {
TOGGLE_SS_RELAY();
TOGGLE_HW_RELAY();
last_micros = micros();

}

}

void TOGGLE_HW_RELAY(){
if (state == 0) {
  digitalWrite(PA15, HIGH);
     // PORTA |= _BV(PA15); //8
      state = 1;
    }
    else {
      digitalWrite(PA15, LOW);
     // PORTA &= ~_BV(PA15); //8
      state = 0;
    }
    TOGGLE_SS_RELAY();
 } 

void TOGGLE_SS_RELAY(){
if (hate == 0) {
    
    digitalWrite(PA12,HIGH);
      hate = 1;
    }
    else {
    digitalWrite(PA12,LOW);
     hate = 0;
    }
  }
  
void CHANGEPRESET() {
  if ((long)(micros() - last_micros2) >= debouncing_time * 1500) {
   
   
    led++;
    if (led >= 7) {
      led = 1;
    }
    last_micros2 = micros();
  }
  switch (led) {
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
void Q(int cally){
if(cally ==1 ){ digitalWrite(PB8,LOW);    digitalWrite(PB9,LOW);  }
if(cally ==2 ){ digitalWrite(PB8,HIGH);    digitalWrite(PB9,LOW);  }
if(cally ==3 ){ digitalWrite(PB8,LOW);    digitalWrite(PB9,HIGH);  }
if(cally ==4 ){ digitalWrite(PB8,HIGH);    digitalWrite(PB9,HIGH);  }
}


void GET_PROFILES(){
switch (led) {
    case 1:
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 0; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 500; //HP CENTRE VOLUME
profilearray[3]  = -(env); //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = profile1[i]; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = profile3[i]; // LP Profile Variable or number

profilearray[8]  = 0; // Freq Profile, Variable or number
profilearray[9]  = 100; //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */

//profilearray[1,1.0,4095,]        
      break;

    case 2:
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 4096; //HP CENTRE VOLUME
profilearray[3]  = 0; //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = 0; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = 0; // LP Profile Variable or number

profilearray[8]  = env; // Freq Profile, Variable or number
profilearray[9]  = 100; //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */
      break;

    case 3:
      /* PROFILE INFO */
//put into array?
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 0; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 2048; //HP CENTRE VOLUME
profilearray[3]  = profile3[i]; //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = 0; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = 0; // LP Profile Variable or number

profilearray[8]  = profile3[i]; // Freq Profile, Variable or number
profilearray[9]  = 100; //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */
      break;

    case 4:     
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 1.0; // envelope depth as decimal
profilearray[2]  = profile1[i]; //  HP Profile Variable or number
profilearray[3]  = 4095; //BP Profile Variable or number
profilearray[4]  = 0; // LP Profile Variable or number
profilearray[5]  = env; // Freq Profile, Variable or number
profilearray[6]  = 1; //Profile of Master
profilearray[7]  = 3000; //higher scope of centre frequency knob
profilearray[8]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */ 
      break;

    case 5:
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 1.0; // envelope depth as decimal
profilearray[2]  = profile1[i]; //  HP Profile Variable or number
profilearray[3]  = 4095; //BP Profile Variable or number
profilearray[4]  = 0; // LP Profile Variable or number
profilearray[5]  = env; // Freq Profile, Variable or number
profilearray[6]  = 1; //Profile of Master
profilearray[7]  = 3000; //higher scope of centre frequency knob
profilearray[8]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */
      break;

    case 6:     
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 1.0; // envelope depth as decimal
profilearray[2]  = profile1[i]; //  HP Profile Variable or number
profilearray[3]  = 4095; //BP Profile Variable or number
profilearray[4]  = 0; // LP Profile Variable or number
profilearray[5]  = env; // Freq Profile, Variable or number
profilearray[6]  = 1; //Profile of Master
profilearray[7]  = 3000; //higher scope of centre frequency knob
profilearray[8]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */
      break;
  }

}






void one() {
  /*-------------------------------------------------------------------------------*/
digitalWrite(PA8, HIGH);
digitalWrite(PB5, LOW);  
}

void two() {
    /*-------------------------------------------------------------------------------*/
digitalWrite(PA9, HIGH);
digitalWrite(PA8, LOW);  
}

void three() {
    /*-------------------------------------------------------------------------------*/
digitalWrite(PA10, HIGH);
digitalWrite(PA9, LOW);  
}

void four() {
    /*-------------------------------------------------------------------------------*/
digitalWrite(PA11, HIGH);
digitalWrite(PA10, LOW);  
}

void five() {
    /*-------------------------------------------------------------------------------*/
digitalWrite(PB4, HIGH);
digitalWrite(PA11, LOW);  
}

void six() {
    /*-------------------------------------------------------------------------------*/
digitalWrite(PB5, HIGH);
digitalWrite(PB4, LOW);  
}