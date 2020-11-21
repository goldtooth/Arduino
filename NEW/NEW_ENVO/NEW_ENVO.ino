#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>

HardwareTimer timer(2);
#define TIMER_RATE 10000

const int thump[512] =
{
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-5,-7,-8,-8,-8,-8,
-8,-9,-10,-11,-12,-13,-15,-16,
-17,-18,-19,-20,-22,-23,-25,-26,
-28,-29,-31,-32,-34,-37,-39,-42,
-44,-45,-46,-46,-47,-48,-50,-51,
-53,-55,-57,-58,-60,-61,-62,-63,
-64,-65,-66,-67,-68,-69,-70,-71,
-72,-73,-74,-75,-76,-77,-78,-79,
-80,-81,-82,-83,-84,-85,-86,-87,
-88,-89,-91,-92,-93,-95,-96,-97,
-99,-100,-101,-102,-103,-104,-105,-106,
-107,-108,-109,-111,-112,-114,-115,-117,
-118,-120,-121,-123,-124,-125,-127,-128,
-129,-130,-131,-132,-133,-134,-135,-136,
-137,-138,-140,-141,-142,-143,-144,-146,
-147,-148,-148,-149,-149,-150,-150,-150,
-151,-151,-152,-152,-152,-153,-153,-153,
-153,-154,-154,-154,-155,-155,-155,-155,
-156,-156,-157,-158,-159,-160,-160,-161,
-162,-163,-164,-164,-165,-165,-166,-166,
-167,-167,-168,-168,-168,-169,-169,-170,
-170,-171,-171,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-172,-172,-172,-172,-172,-172,-172,-172,
-171,-169,-168,-167,-165,-164,-162,-161,
-159,-158,-156,-155,-153,-152,-151,-149,
-148,-147,-145,-144,-143,-141,-140,-139,
-137,-136,-134,-131,-129,-127,-125,-122,
-120,-117,-115,-112,-109,-107,-104,-102,
-99,-97,-95,-93,-90,-88,-87,-86,
-85,-83,-82,-81,-80,-78,-76,-74,
-72,-70,-68,-66,-65,-63,-61,-59,
-58,-56,-55,-54,-53,-51,-50,-49,
-48,-46,-43,-41,-39,-37,-34,-32,
-30,-29,-27,-25,-23,-22,-20,-18,
-17,-15,-14,-12,-11,-9,-8,-7,
-7,-6,-6,-5,-5,-4,-3,-1,
0,1,3,4,5,7,8,9,
10,11,13,14,15,16,17,17,
18,18,19,19,20,21,22,23,
25,26,27,28,30,31,33,34,
36,37,39,40,41,42,43,44,
45,46,47,48,49,51,52,53,
55,56,57,59,60,61,63,64,
65,67,68,69,70,71,73,74,
75,76,77,79,80,81,83,84,
85,87,88,89,89,90,90,91,
91,92,92,93,93,94,95,95,
96,96,97,97,98,98,98,99,
99,100,100,101,101,102,103,103,
104,104,104,104,104,104,104,104};




const int lightsag[512] =
{

0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,4,4,4,4,4,3,
2,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,-1,-2,-3,
-4,-4,-4,-4,-4,-4,-4,-4,
-4,-4,-4,-4,-4,-5,-6,-6,
-7,-8,-9,-10,-10,-11,-12,-12,
-12,-12,-12,-12,-13,-14,-15,-16,
-16,-16,-16,-16,-16,-17,-19,-20,
-20,-20,-20,-20,-20,-20,-20,-20,
-24,-25,-27,-28,-28,-28,-28,-28,
-29,-30,-31,-32,-32,-32,-32,-33,
-34,-35,-36,-36,-36,-36,-38,-40,
-42,-43,-45,-46,-48,-49,-51,-52,
-53,-55,-56,-57,-58,-59,-61,-62,
-63,-64,-65,-66,-66,-67,-68,-69,
-71,-72,-73,-75,-76,-77,-78,-79,
-80,-82,-83,-85,-86,-88,-90,-91,
-93,-94,-96,-98,-101,-103,-106,-108,
-109,-110,-111,-113,-114,-115,-116,-118,
-120,-122,-124,-126,-129,-131,-134,-136,
-138,-140,-142,-144,-146,-148,-149,-151,
-152,-154,-155,-157,-158,-160,-161,-163,
-164,-166,-167,-169,-170,-172,-173,-174,
-175,-176,-177,-178,-178,-179,-180,-181,
-182,-182,-183,-184,-186,-187,-189,-190,
-192,-193,-194,-194,-195,-196,-198,-200,
-201,-202,-203,-204,-205,-207,-208,-208,
-208,-208,-208,-209,-211,-212,-213,-215,
-216,-216,-216,-216,-216,-216,-217,-218,
-219,-220,-220,-220,-220,-221,-223,-224,
-224,-224,-224,-224,-224,-224,-224,-224,
-226,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-228,-228,
-228,-228,-228,-228,-228,-228,-227,-226,
-225,-224,-223,-222,-221,-220,-219,-217,
-216,-215,-214,-214,-213,-212,-211,-209};





const int rise[512] = { 
-1960,-1956,-1951,-1947,-1942,-1937,-1933,-1928,
-1923,-1918,-1912,-1907,-1902,-1897,-1892,-1886,
-1881,-1876,-1869,-1863,-1856,-1849,-1843,-1836,
-1829,-1823,-1816,-1809,-1803,-1796,-1789,-1781,
-1774,-1767,-1759,-1752,-1745,-1737,-1730,-1723,
-1715,-1708,-1701,-1693,-1686,-1679,-1671,-1664,
-1656,-1648,-1641,-1633,-1625,-1617,-1610,-1602,
-1594,-1586,-1578,-1571,-1563,-1555,-1547,-1540,
-1532,-1524,-1517,-1510,-1503,-1496,-1490,-1483,
-1476,-1469,-1462,-1455,-1448,-1441,-1434,-1428,
-1421,-1414,-1407,-1400,-1392,-1384,-1376,-1368,
-1360,-1352,-1344,-1336,-1328,-1320,-1312,-1304,
-1296,-1288,-1280,-1272,-1264,-1255,-1247,-1238,
-1230,-1221,-1213,-1204,-1196,-1187,-1179,-1170,
-1162,-1153,-1145,-1136,-1126,-1117,-1107,-1098,
-1088,-1078,-1069,-1059,-1050,-1040,-1030,-1021,
-1011,-1002,-992,-983,-974,-965,-956,-947,
-938,-929,-920,-911,-902,-894,-885,-876,
-867,-858,-849,-840,-831,-822,-813,-804,
-795,-787,-778,-770,-761,-753,-744,-736,
-727,-719,-710,-702,-693,-685,-676,-669,
-662,-655,-648,-641,-634,-627,-620,-613,
-606,-599,-592,-585,-578,-571,-564,-557,
-551,-544,-537,-531,-524,-517,-511,-504,
-497,-491,-484,-477,-471,-464,-457,-450,
-443,-436,-429,-422,-414,-407,-400,-393,
-386,-379,-372,-365,-359,-352,-346,-339,
-333,-326,-320,-313,-307,-300,-294,-288,
-281,-275,-269,-263,-257,-251,-244,-238,
-232,-226,-220,-215,-209,-203,-197,-192,
-186,-180,-176,-171,-167,-163,-158,-154,
-150,-145,-141,-137,-132,-128,-125,-122,
-120,-117,-114,-111,-108,-106,-103,-100,
-98,-96,-94,-93,-91,-89,-87,-85,
-83,-81,-79,-78,-76,-74,-72,-71,
-70,-68,-67,-66,-65,-64,-62,-61,
-60,-59,-58,-56,-55,-54,-53,-52,
-50,-49,-48,-47,-46,-44,-43,-42,
-41,-40,-38,-37,-36,-35,-35,-34,
-34,-33,-32,-32,-31,-30,-30,-29,
-29,-28,-27,-27,-26,-26,-25,-25,
-24,-24,-23,-22,-22,-21,-21,-20,
-20,-19,-19,-18,-17,-17,-16,-16,
-15,-15,-14,-14,-13,-12,-12,-11,
-11,-10,-10,-9,-9,-8,-7,-7,
-6,-6,-5,-5,-4,-4,-3,-3,
-2,-1,-1,0,0,1,1,2,
2,3,3,4,5,5,6,7,
8,8,9,10,10,11,12,12,
13,14,15,15,16,17,17,18,
18,19,19,20,20,21,21,22,
22,23,23,24,25,25,26,27,
28,28,29,30,30,31,32,32,
33,34,35,35,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36,
36,36,36,36,36,36,36,36};


const int compress[512] = { 
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-8,-8,-8,-8,-8,
-8,-8,-8,-9,-10,-10,-11,-12,
-13,-14,-14,-15,-16,-17,-17,-18,
-19,-20,-20,-21,-22,-23,-23,-24,
-26,-27,-29,-30,-32,-33,-34,-34,
-35,-36,-37,-38,-39,-40,-42,-43,
-45,-46,-48,-50,-53,-55,-57,-59,
-62,-64,-66,-67,-69,-71,-73,-74,
-76,-77,-79,-80,-82,-83,-85,-86,
-88,-90,-91,-93,-95,-97,-98,-100,
-102,-103,-105,-107,-109,-110,-112,-113,
-114,-115,-117,-118,-119,-120,-120,-121,
-121,-122,-122,-123,-123,-124,-125,-126,
-128,-129,-130,-131,-133,-134,-135,-136,
-138,-139,-140,-140,-141,-141,-142,-142,
-143,-143,-144,-145,-146,-147,-148,-148,
-149,-150,-151,-152,-152,-153,-153,-154,
-154,-155,-155,-156,-156,-157,-157,-158,
-158,-159,-159,-160,-160,-160,-161,-161,
-162,-162,-163,-163,-164,-164,-165,-166,
-167,-168,-169,-170,-171,-172,-172,-173,
-173,-174,-174,-175,-175,-176,-176,-177,
-177,-178,-178,-179,-179,-180,-180,-180,
-181,-181,-182,-182,-183,-183,-184,-185,
-186,-186,-187,-188,-189,-190,-190,-191,
-192,-192,-193,-193,-193,-194,-194,-195,
-195,-195,-196,-196,-197,-197,-198,-199,
-200,-200,-201,-202,-203,-203,-204,-204,
-205,-205,-206,-206,-207,-207,-208,-208,
-209,-209,-210,-210,-211,-211,-212,-212,
-213,-213,-213,-214,-214,-215,-215,-215,
-216,-216,-217,-218,-218,-219,-220,-221,
-222,-222,-223,-224,-224,-224,-224,-224,
-224,-224,-224,-224,-224,-224,-225,-225,
-226,-226,-226,-227,-227,-228,-228,-229,
-230,-230,-231,-232,-233,-234,-234,-235,
-236,-236,-237,-237,-238,-238,-239,-239,
-240,-240,-240,-241,-241,-242,-242,-243,
-243,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-244,
-244,-244,-244,-244,-244,-244,-244,-243,
-241,-240,-240,-240,-240,-240,-240,-240,
-240,-240,-240,-240,-240,-240,-240,-240,
-240,-240,-240,-240,-240,-240,-240,-240};


const int compress2[512] = { 
-16,-16,-16,-16,-16,-16,-17,-17,
-18,-19,-19,-20,-21,-22,-23,-25,
-26,-27,-28,-29,-29,-30,-31,-31,
-32,-33,-34,-34,-35,-36,-36,-36,
-36,-36,-36,-37,-37,-38,-39,-39,
-40,-41,-42,-43,-44,-45,-46,-46,
-47,-48,-49,-49,-50,-51,-51,-52,
-54,-55,-57,-58,-60,-62,-63,-65,
-66,-68,-68,-68,-69,-70,-72,-73,
-74,-75,-76,-78,-79,-80,-81,-83,
-84,-85,-87,-88,-90,-91,-93,-94,
-96,-97,-99,-100,-101,-103,-104,-105,
-106,-107,-108,-110,-112,-114,-116,-117,
-118,-119,-120,-121,-122,-122,-123,-124,
-127,-129,-132,-133,-135,-136,-138,-140,
-142,-145,-147,-150,-152,-152,-152,-155,
-157,-160,-162,-163,-165,-166,-168,-170,
-171,-173,-174,-176,-178,-180,-182,-184,
-185,-187,-188,-189,-191,-192,-196,-200,
-203,-205,-208,-210,-212,-214,-216,-217,
-218,-219,-220,-221,-222,-223,-224,-227,
-229,-232,-232,-232,-232,-235,-237,-240,
-241,-243,-244,-246,-248,-250,-252,-254,
-255,-257,-258,-260,-261,-263,-264,-266,
-268,-270,-272,-272,-272,-272,-273,-275,
-276,-278,-280,-281,-282,-283,-284,-285,
-286,-286,-287,-288,-290,-292,-293,-294,
-295,-296,-296,-296,-297,-299,-300,-302,
-304,-304,-304,-304,-307,-309,-312,-312,
-312,-312,-315,-317,-320,-320,-320,-320,
-322,-323,-325,-326,-328,-328,-328,-328,
-328,-331,-333,-336,-336,-336,-336,-336,
-336,-337,-338,-339,-340,-341,-342,-343,
-344,-344,-344,-344,-344,-345,-346,-347,
-348,-348,-348,-348,-349,-350,-351,-352,
-352,-352,-352,-353,-354,-354,-355,-356,
-356,-356,-356,-356,-357,-359,-360,-360,
-360,-360,-360,-360,-360,-361,-363,-364,
-364,-364,-364,-364,-364,-364,-364,-364,
-364,-364,-364,-364,-364,-364,-364,-364,
-364,-364,-364,-364,-364,-364,-364,-364,
-364,-364,-364,-364,-364,-364,-364,-364,
-364,-364,-364,-364,-363,-362,-362,-361,
-360,-360,-360,-360,-360,-358,-356,-354,
-352,-351,-350,-349,-348,-347,-346,-345,
-344,-343,-342,-342,-341,-340,-339,-337,
-336,-335,-334,-334,-333,-332,-331,-330,
-330,-329,-328,-327,-325,-324,-322,-321,
-319,-318,-316,-315,-313,-312,-312,-312,
-308,-307,-305,-304,-302,-300,-298,-296,
-295,-293,-292,-289,-287,-284,-284,-284,
-280,-276,-275,-274,-273,-272,-270,-268,
-265,-263,-260,-258,-256,-253,-251,-248,
-247,-245,-244,-240,-238,-236,-234,-232,
-228,-226,-224,-221,-219,-216,-212,-208,
-204,-200,-196,-194,-192,-190,-188,-184,
-180,-178,-176,-172,-170,-168,-164,-160,
-160,-158,-156,-152,-149,-147,-144,-140,
-136,-132,-128,-124,-121,-119,-116,-114,
-112,-108,-104,-102,-100,-96,-92,-88,
-84,-80,-76,-72,-68,-68,-68,-65,
-63,-60,-58,-56,-54,-52,-50,-48,
-46,-44,-42,-40,-39,-37,-36,-34,
-32,-32,-32,-32,-32,-30,-28,-27};



const int compress3[512] = { 
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,-8,-8,
-12,-16,-19,-22,-25,-27,-30,-33,
-36,-38,-39,-41,-43,-45,-46,-48,
-51,-54,-57,-59,-62,-65,-68,-70,
-73,-75,-77,-79,-82,-84,-87,-90,
-94,-97,-100,-103,-107,-110,-113,-117,
-120,-124,-128,-132,-136,-140,-143,-147,
-150,-154,-157,-161,-164,-168,-172,-176,
-180,-184,-188,-191,-195,-198,-202,-205,
-209,-212,-216,-220,-224,-228,-232,-236,
-240,-245,-250,-254,-259,-264,-268,-273,
-277,-282,-286,-291,-295,-300,-306,-312,
-318,-324,-330,-336,-341,-347,-352,-357,
-363,-368,-372,-377,-381,-386,-390,-395,
-399,-404,-408,-413,-417,-422,-426,-431,
-435,-440,-446,-452,-458,-464,-470,-476,
-480,-485,-489,-494,-498,-503,-507,-512,
-516,-521,-527,-532,-537,-543,-548,-554,
-560,-566,-572,-578,-584,-588,-593,-597,
-602,-606,-611,-615,-620,-625,-630,-635,
-641,-646,-651,-656,-661,-666,-671,-676,
-680,-685,-690,-695,-700,-705,-709,-714,
-718,-723,-727,-732,-737,-741,-746,-750,
-755,-759,-764,-768,-771,-775,-779,-782,
-786,-790,-793,-797,-801,-804,-808,-812,
-815,-819,-823,-826,-830,-834,-837,-841,
-845,-848,-852,-856,-860,-864,-868,-872,
-876,-880,-884,-888,-892,-896,-900,-905,
-909,-914,-918,-922,-927,-931,-936,-940,
-945,-951,-956,-961,-967,-972,-978,-984,
-990,-996,-1002,-1008,-1013,-1018,-1023,-1029,
-1034,-1039,-1044,-1049,-1055,-1060,-1065,-1071,
-1076,-1082,-1087,-1093,-1099,-1105,-1110,-1116,
-1122,-1127,-1133,-1138,-1144,-1150,-1156,-1162};






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
float dong;
bool wrong = false;
int Adel;
int deep;
int profilearray[13];
int trigger = 0;
bool flip = false;
int shift;
int timer1_counter;
float gate;
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
int offset = 0;
int BAWIN; //adjust to allow for bias of audio input 
int maxVal = 0;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
int  pro_env_more;
float env_sens;
float env;
int lvl       = 10;      // Current "dampened" audio level
int minLvlAvg = 0;     // For dynamic adjustment of graph low & high
int maxLvlAvg = 4096;
int value=0;
int maxAudioValue = 0;          // Maximum analog value read from audio channel
int ledLevel;
//const int ledCount = 255;
const int numReadings = 20;
int readings[numReadings];
float peakValue = 1;
int lastPeak = 1;
int check;
int digger;
float release_f;
int attack_f;
#define NOISE     10 // Noise/hum/interference in mic signal
float env_duty;

//-->DECLARE  THINGS



const int ENVA_NUMSAMPLE = 400; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int ENVA_SAMP[ENVA_NUMSAMPLE] = {0}; //value of sample
int ENVA_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int ENVA_TOTAL = 0; //running total of sample values
const int BENVA_NUMSAMPLE = 200; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int BENVA_SAMP[BENVA_NUMSAMPLE] = {0}; //value of sample
int BENVA_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int BENVA_TOTAL = 0; //running total of sample values

int shorterval;
float envattack;
float env_depth = 1.0;
int listener = 5;
int avg = 10;//was 50
int larger_avg = 30;
int pro_env;
bool env_up = false;
int HP_VOL_PROFILE ;
int BP_VOL_PROFILE ;
int LP_VOL_PROFILE ; 
int FREQ_PROFILE ;
int MASTER_PROFILE;

int selection[5] = {0,0,0,0,0}; 
float Master;
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
  attachInterrupt(PB13, CHANGEPRESET, FALLING); // or rising / or change CHANGEPRESET
  pinMode(PB12, INPUT_PULLUP);
  attachInterrupt(PB12, CHANGEPRESET, FALLING); // or rising / or change ONANDOFF
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


one();
}

void loop() {

val = (val <= NOISE) ? 0 : (val - NOISE);     // Remove noise/hum
  // Remove noise/hum


//why is this not working???



peakValue = val;
//lastPeak = millis();
//trigger = 0;
//check = 1002;
trigger= shorterval/8;
digger= val/8;
//check = peakValue; 
peakValue = peakValue - release_f;
env_duty =  peakValue;
env_duty = map(env_duty , 0, 4096, profilearray[12], profilearray[0]);

GET_PROFILES();



Master = 1-(((profilearray[9]-2048)/2048));


OUT1 = (profilearray[2] + (profilearray[3]*depth))*Master; //HP
OUT2 = (profilearray[4] + (profilearray[5]*depth))*Master; 
OUT3 = (profilearray[6] + (profilearray[7]*depth))*Master; 
OUT4 = centre + (profilearray[8]*depth);
//Master = (profilearray[9]*depth);

//THELISTENER ==true



if (OUT1>4095){OUT1 =4096;}if (OUT1<1){OUT1 =0;}
if (OUT2>4095){OUT2 =4096;}if (OUT2<1){OUT2 =0;}
if (OUT3>4095){OUT3 =4096;}if (OUT3<1){OUT3 =0;}
if (OUT4>4095){OUT4 =4096;}if (OUT4<1){OUT4 =0;}

OUT1 = ((0-OUT1)+4096);
OUT2 = ((0-OUT2)+4096);
OUT3 = ((0-OUT3)+4096);
OUT4 = ((0-OUT4)+4096);

//delayMicroseconds(actdelay);
dac.analogWrite(OUT1,OUT2,OUT3,OUT4);
//--------------- HP, MID, LP, FREQ
//-------------4095 is off,  0 is full



/* if (check  > 500){
  digitalWrite(PB5, HIGH);
}else{digitalWrite(PB5, LOW);}

if (check  > 1000){
  digitalWrite(PB4, HIGH);
}else{digitalWrite(PB4, LOW);}


if (check   > 2000){
  digitalWrite(PA11, HIGH);
}else{digitalWrite(PA11, LOW);}


if (check > 3000){
  digitalWrite(PA10, HIGH);
}else{digitalWrite(PA10, LOW);}

if (check  > 4000){
  digitalWrite(PA9, HIGH);
}else{digitalWrite(PA9, LOW);}

*/



}


void handler(void) {
 checkthings();
}; 





void checkthings(){

/*RELEASE */
release_f = analogRead(PA2); // change to relase / smoothing
release_f = map(release_f , 0, 4095, 2500, 1);
//gate = gate/100;
/*ATTACK */
attack_f = analogRead(PA4); // attack
attack_f = map(attack_f, 0, 4095, 20, 100);
//attack = map(attack, 0, 4095, 0, 4000);




/*DEPTH */
depth = analogRead(PA3); // depth
depth = map(depth, 0, 4095, 1, 100);
depth = depth/100;

/*SENSITIVITY */
sens = analogRead(PA1); // chanege to sens


/*CENTRE FREQUENCY */
centre = analogRead(PA5); //centre freq    
centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);
/*AUDIO IN */
RAWIN = analogRead(PA0); // AUDIOI;
BAWIN = RAWIN;
RAWIN = map(RAWIN,0,4096, 0, sens);
 

ENVA_TOTAL -=  ENVA_SAMP[ENVA_INDEX]; //Subtracts the last sample
ENVA_SAMP[ENVA_INDEX] = RAWIN; //sets the sample to the analoge reading (0-1024)
ENVA_TOTAL += ENVA_SAMP[ENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (ENVA_INDEX >= attack_f){ //checks if limit of samples is reached
    ENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
if (ENVA_INDEX >= ENVA_NUMSAMPLE-2){ //checks if limit of samples is reached
    ENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
val = (ENVA_TOTAL / attack_f);

BENVA_TOTAL -=  BENVA_SAMP[BENVA_INDEX]; //Subtracts the last sample
BENVA_SAMP[BENVA_INDEX] = BAWIN; //sets the sample to the analoge reading (0-1024)
BENVA_TOTAL += BENVA_SAMP[BENVA_INDEX++]; //adds the sample to the total and moves INDEX to the next value
if (BENVA_INDEX >= (10)){ //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  }
 if (BENVA_INDEX >= BENVA_NUMSAMPLE-2){ //checks if limit of samples is reached
    BENVA_INDEX = 0; //resets INDEX to 0, restarting the process
  } 
shorterval = (BENVA_TOTAL / (10));







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
    /* PROFILE INFO */
      /* PROFILE INFO */

Q(1); //1, 3, 4, 2; 1 is stronge
//gated?
profilearray[1]  = 100; // 1 yes  - 0 no

profilearray[2]  = 0;//4096-env_duty; //HP CENTRE VOLUME
profilearray[3]  = 0;  //  HP Profile Variable or number

profilearray[4]  = 4096; // BP CENTRE VOLUME
profilearray[5]  = lightsag[trigger];//env_duty; //BP Profile Variable or number

profilearray[6]  = 2000; // LP CENTRE VOLUME
profilearray[7]  = -lightsag[trigger];  // LP Profile Variable or number


//FREQUENCY KNOB
profilearray[10]  = 4096; //higher scope of centre frequency knob*4
profilearray[11]  = 0; //lower scope of centre frequency knob

//FREQUENCY AUDIO
profilearray[8]  = env_duty; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[0] = 0; // top of the audio envelope
profilearray[12] = 1000; // BOttom of the audio envelope

//MASTER_PROFILE
profilearray[9]  = 1; //Profile of Master
break;


    

    case 2:
Q(4); //1, 3, 4, 2; 1 is stronge
//gated?
profilearray[1]  = 100; // 1 yes  - 0 no

profilearray[2]  = 1000;//2000;//4096-env_duty; //HP CENTRE VOLUME
profilearray[3]  = -rise[digger];//compress3[trigger];   //  HP Profile Variable or number

profilearray[4]  = 4096; // BP CENTRE VOLUME
profilearray[5]  = compress3[trigger];//env_duty; //BP Profile Variable or number

profilearray[6]  = 1000;//4096; // LP CENTRE VOLUME
profilearray[7]  = -compress3[digger];//compress3[trigger];  // LP Profile Variable or number


//FREQUENCY KNOB
profilearray[10]  = 4000; //higher scope of centre frequency knob*4
profilearray[11]  = 1000; //lower scope of centre frequency knob

//FREQUENCY AUDIO
profilearray[8]  = compress3[digger]; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[0] = 4090; // top of the audio envelope
profilearray[12] = 0; // BOttom of the audio envelope

//MASTER_PROFILE
profilearray[9]  = 1; //Profile of Master
break;

    case 3:

Q(1); //1, 3, 4, 2; 1 is stronge
//gated?
profilearray[1]  = 100; // 1 yes  - 0 no

profilearray[2]  = 2000;//4096-env_duty; //HP CENTRE VOLUME
profilearray[3]  = compress3[trigger];   //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = 0;//env_duty; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = compress3[trigger];  // LP Profile Variable or number


//FREQUENCY KNOB
profilearray[10]  = 4500; //higher scope of centre frequency knob*4
profilearray[11]  = 2000; //lower scope of centre frequency knob

//FREQUENCY AUDIO
profilearray[8]  = env_duty; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[0] = 2500; // top of the audio envelope
profilearray[12] = 0; // BOttom of the audio envelope

//MASTER_PROFILE
profilearray[9]  = 1; //Profile of Master
break;

    case 4:     
    /* PROFILE INFO */
      /* PROFILE INFO */
Q(4); //1, 3, 4, 2; 1 is stronge
//gated?
profilearray[1]  = 100; // 1 yes  - 0 no

profilearray[2]  = 2000;//4096-env_duty; //HP CENTRE VOLUME
profilearray[3]  = compress3[trigger];   //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = 0;//env_duty; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = compress3[trigger];  // LP Profile Variable or number


//FREQUENCY KNOB
profilearray[10]  = 4000; //higher scope of centre frequency knob*4
profilearray[11]  = 1000; //lower scope of centre frequency knob

//FREQUENCY AUDIO
profilearray[8]  = env_duty; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[0] = 2500; // top of the audio envelope
profilearray[12] = 0; // BOttom of the audio envelope

//MASTER_PROFILE
profilearray[9]  = 1; //Profile of Master
break;

    case 5:
      /* PROFILE INFO */
Q(3); //1, 3, 4, 2; 1 is stronge
//gated?
profilearray[1]  = 100; // 1 yes  - 0 no

profilearray[2]  = 3000;//4096-env_duty; //HP CENTRE VOLUME
profilearray[3]  = compress3[digger];   //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = env_duty;//env_duty; //BP Profile Variable or number

profilearray[6]  = 1000; // LP CENTRE VOLUME
profilearray[7]  = 0;  // LP Profile Variable or number


//FREQUENCY KNOB
profilearray[10]  = 4000; //higher scope of centre frequency knob*4
profilearray[11]  = 1000; //lower scope of centre frequency knob

//FREQUENCY AUDIO
profilearray[8]  = 0; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[0] = 2500 ; // top of the audio envelope
profilearray[12] = 0; // BOttom of the audio envelope

//MASTER_PROFILE
profilearray[9]  = 1; //Profile of Master

    case 6:     
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 1.0; // envelope depth as decimal
profilearray[2]  = 0; //  HP Profile Variable or number
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
//digitalWrite(PB5, LOW);  
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
//digitalWrite(PB5, HIGH);
//digitalWrite(PB4, LOW);  
}
