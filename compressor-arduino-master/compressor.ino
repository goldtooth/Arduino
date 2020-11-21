#include <mcp4728.h>
#include <Wire.h>
mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0
#include <math.h>

HardwareTimer timer(2);
#define TIMER_RATE 100


const uint16_t profile1[512] =
{
4040,4040,4038,4036,4033,4030,4027,4024,
4023,4021,4020,4018,4016,4012,4008,4006,
4004,4002,4000,3996,3992,3988,3987,3985,
3984,3976,3974,3972,3970,3968,3965,3963,
3960,3957,3955,3952,3948,3944,3941,3939,
3936,3932,3928,3924,3920,3916,3912,3908,
3904,3900,3896,3890,3884,3882,3880,3872,
3869,3867,3864,3858,3852,3848,3844,3840,
3836,3832,3824,3822,3820,3812,3808,3804,
3800,3796,3792,3788,3784,3780,3776,3772,
3768,3764,3762,3760,3752,3751,3749,3748,
3744,3740,3738,3736,3734,3732,3728,3724,
3724,3721,3719,3716,3708,3706,3704,3703,
3701,3700,3696,3692,3692,3692,3688,3688,
3685,3683,3680,3680,3680,3676,3676,3676,
3672,3668,3667,3665,3664,3664,3660,3660,
3660,3656,3656,3652,3652,3652,3648,3648,
3646,3644,3640,3636,3634,3632,3630,3628,
3624,3620,3620,3616,3616,3612,3612,3608,
3608,3608,3608,3606,3604,3604,3604,3600,
3600,3600,3596,3596,3596,3594,3592,3591,
3589,3588,3588,3588,3585,3583,3580,3579,
3577,3576,3574,3573,3571,3570,3568,3564,
3562,3560,3560,3558,3556,3554,3552,3548,
3548,3548,3548,3544,3543,3541,3540,3537,
3535,3532,3529,3527,3524,3522,3520,3518,
3516,3514,3512,3510,3508,3506,3504,3502,
3500,3496,3492,3492,3492,3492,3489,3487,
3484,3482,3480,3478,3476,3472,3468,3467,
3465,3464,3460,3456,3452,3448,3446,3444,
3442,3440,3438,3436,3430,3424,3420,3416,
3412,3408,3404,3400,3396,3392,3388,3380,
3376,3372,3356,3348,3340,3324,3316,3308,
3300,3276,3262,3248,3216,3176,3146,3116,
3092,3068,3044,3006,2968,2926,2884,2844,
2804,2748,2692,2636,2578,2520,2456,2392,
2359,2326,2293,2260,2235,2210,2186,2161,
2136,2086,2036,2000,1964,1928,1893,1859,
1824,1770,1716,1674,1632,1590,1548,1520,
1508,1500,1492,1476,1469,1462,1455,1448,
1441,1435,1428,1414,1400,1391,1381,1372,
1356,1340,1308,1292,1276,1265,1255,1244,
1232,1220,1206,1192,1178,1164,1140,1131,
1121,1112,1102,1092,1087,1081,1076,1068,
1060,1055,1049,1044,1039,1034,1029,1024,
1020,1016,1012,1008,1004,1000,996,992,
988,984,979,973,968,965,963,960,
957,954,951,949,946,943,940,936,
932,928,924,922,919,917,914,912,
909,907,904,901,899,896,893,890,
886,883,880,878,875,873,870,868,
865,863,860,857,855,852,850,849,
847,846,844,842,839,837,835,833,
830,828,826,825,823,822,820,817,
815,812,810,809,807,806,804,803,
801,800,799,798,797,796,795,793,
792,791,790,789,788,787,786,785,
784,784,784,784,784,784,784,784,
784,784,784,784,784,784,784,785,
786,787,788,788,788,788,788,788,
788,788,788,788,788,789,790,790,
791,792,793,794,794,795,796,796,
796,796,796,800,800,801,803,804,
804,804,804,804,804,804,805,805};


const uint16_t profile2[512] =
{

4056,4052,4047,4041,4036,4028,4020,4016,
4012,4008,4005,4002,3999,3996,3988,3980,
3975,3969,3964,3958,3952,3944,3936,3931,
3926,3921,3916,3908,3900,3895,3889,3884,
3877,3870,3863,3856,3849,3843,3836,3828,
3820,3812,3807,3801,3796,3791,3785,3780,
3774,3769,3763,3758,3752,3745,3738,3730,
3723,3716,3711,3707,3702,3698,3693,3689,
3684,3678,3671,3665,3658,3652,3647,3642,
3637,3632,3627,3622,3618,3613,3608,3603,
3598,3593,3588,3582,3576,3570,3564,3558,
3551,3545,3538,3532,3527,3522,3517,3512,
3507,3502,3498,3493,3488,3483,3478,3473,
3468,3463,3458,3453,3447,3442,3437,3432,
3426,3421,3415,3410,3404,3399,3394,3389,
3383,3378,3373,3368,3361,3354,3347,3340,
3335,3329,3324,3312,3308,3324,3318,3312,
3308,3304,3300,3293,3287,3280,3274,3268,
3260,3252,3246,3240,3235,3229,3224,3214,
3204,3196,3188,3180,3175,3170,3165,3160,
3149,3139,3128,3119,3110,3101,3092,3085,
3077,3070,3063,3055,3048,3039,3030,3022,
3013,3004,2984,2964,2954,2943,2933,2922,
2912,2901,2891,2880,2873,2867,2860,2853,
2847,2840,2831,2823,2814,2805,2797,2788,
2779,2770,2761,2752,2743,2733,2724,2718,
2712,2704,2700,2696,2692,2688,2684,2680,
2675,2670,2666,2661,2656,2652,2648,2644,
2640,2636,2630,2624,2618,2612,2607,2601,
2596,2591,2586,2581,2576,2571,2566,2561,
2556,2549,2543,2536,2526,2516,2511,2506,
2501,2495,2490,2485,2480,2474,2468,2462,
2456,2450,2444,2438,2432,2426,2421,2415,
2410,2404,2396,2390,2384,2368,2354,2340,
2328,2316,2298,2280,2269,2259,2248,2236,
2224,2212,2200,2188,2177,2167,2156,2150,
2143,2137,2130,2124,2113,2103,2092,2081,
2070,2059,2048,2040,2032,2024,2016,2008,
2000,1990,1981,1971,1962,1952,1942,1932,
1922,1912,1902,1892,1882,1872,1862,1852,
1843,1834,1825,1816,1807,1797,1788,1779,
1770,1761,1752,1742,1732,1723,1713,1703,
1693,1683,1673,1664,1654,1644,1635,1626,
1618,1609,1600,1591,1582,1574,1565,1556,
1548,1541,1533,1526,1518,1511,1503,1496,
1488,1478,1469,1459,1450,1440,1430,1420,
1416,1412,1404,1392,1388,1356,1348,1328,
1312,1300,1294,1288,1272,1264,1256,1248,
1240,1232,1220,1212,1204,1196,1188,1184,
1180,1170,1160,1154,1148,1140,1132,1128,
1124,1116,1108,1100,1095,1089,1084,1076,
1068,1060,1053,1047,1040,1028,1016,1010,
1004,998,992,986,979,973,966,960,
951,941,932,921,911,900,887,873,
860,844,828,814,800,790,780,772,
764,756,742,728,716,704,696,688,
680,674,668,662,656,649,643,636,
626,616,607,597,588,568,558,548,
540,532,524,515,505,496,484,472,
460,448,416,400,384,376,367,358,
349,340,312,300,282,276,268,260,
252,244,236,228,220,210,200,180,
176,156,144,136,128,108,92,40,
33,27,20,0,0,0,0,0};



const uint16_t profile3[512] = { 
2048,2004,1992,1988,1984,1972,1966,1960,
1948,1942,1937,1931,1926,1920,1908,1896,
1884,1876,1868,1860,1851,1841,1832,1820,
1808,1799,1789,1780,1762,1744,1720,1704,
1688,1670,1652,1636,1620,1600,1552,1520,
1504,1492,1472,1456,1452,1448,1440,1432,
1426,1420,1416,1408,1404,1404,1402,1400,
1400,1400,1400,1400,1400,1400,1400,1400,
1400,1400,1400,1400,1400,1402,1403,1405,
1407,1409,1410,1412,1415,1417,1420,1423,
1425,1428,1430,1432,1434,1436,1439,1441,
1444,1447,1450,1453,1456,1460,1464,1468,
1474,1480,1483,1485,1488,1494,1500,1504,
1508,1514,1520,1523,1525,1528,1536,1544,
1547,1549,1552,1564,1572,1580,1586,1592,
1600,1608,1618,1628,1636,1644,1668,1682,
1696,1712,1728,1748,1768,1792,1816,1830,
1844,1880,1900,1920,1964,1992,2020,2034,
2048,2066,2084,2102,2120,2146,2172,2220,
2248,2265,2283,2300,2314,2328,2341,2355,
2368,2383,2397,2412,2442,2472,2520,2546,
2572,2620,2637,2655,2724,2768,2876,2944,
2974,3004,3020,3036,3052,3076,3100,3119,
3137,3156,3180,3204,3248,3259,3270,3281,
3292,3308,3324,3331,3338,3345,3352,3358,
3365,3371,3378,3384,3390,3396,3402,3408,
3412,3416,3420,3424,3428,3432,3436,3440,
3444,3448,3451,3453,3456,3459,3461,3464,
3466,3468,3469,3471,3473,3475,3476,3478,
3480,3481,3482,3483,3485,3486,3487,3488,
3489,3491,3492,3493,3495,3496,3497,3498,
3499,3500,3500,3500,3500,3500,3500,3500,
3500,3500,3500,3500,3498,3496,3494,3492,
3490,3488,3486,3483,3481,3478,3476,3473,
3471,3468,3465,3462,3458,3455,3452,3449,
3446,3442,3439,3436,3432,3428,3424,3420,
3416,3410,3405,3399,3394,3388,3381,3374,
3366,3359,3352,3344,3336,3328,3320,3312,
3305,3298,3290,3283,3276,3267,3258,3249,
3240,3228,3216,3204,3195,3187,3178,3169,
3161,3152,3141,3130,3118,3107,3096,3084,
3072,3060,3048,3037,3025,3014,3003,2991,
2980,2966,2952,2938,2924,2909,2894,2879,
2864,2852,2840,2828,2816,2804,2789,2774,
2759,2744,2725,2707,2688,2676,2664,2652,
2640,2626,2612,2598,2584,2568,2552,2536,
2524,2512,2500,2488,2475,2461,2448,2435,
2421,2408,2392,2376,2360,2352,2344,2336,
2328,2319,2309,2300,2291,2281,2272,2265,
2258,2251,2244,2236,2228,2220,2210,2200,
2195,2189,2184,2172,2164,2156,2151,2145,
2140,2135,2129,2124,2118,2112,2108,2104,
2100,2096,2092,2088,2084,2080,2076,2072,
2068,2064,2060,2057,2055,2052,2049,2047,
2044,2042,2040,2038,2036,2033,2031,2028,
2026,2024,2022,2020,2018,2016,2014,2012,
2011,2010,2009,2008,2007,2007,2006,2005,
2005,2004,2004,2004,2004,2004,2004,2004,
2003,2002,2001,2000,2000,2000,2000,1999,
1997,1996,1996,1996,1996,1996,1996,1996,
1996,1996,1996,1996,1997,1999,2000,2002,
2004,2007,2009,2012,2015,2017,2020,2022,
2024,2028,2032,2036,2040,2040,2044,2044,
2044,2048,2048,2048,2050,2052,2052,2052};
const uint16_t profile4[512] = { 
4040,4036,4032,4028,4024,4020,3176,3204,
3244,3248,3144,3140,3136,3132,3132,3128,
3124,3120,3116,3108,3104,3100,3096,3092,
3088,3086,3084,3080,3080,3080,3076,3072,
3072,3068,3068,3068,3068,3064,3060,3056,
3052,3044,3040,3028,3020,3012,2996,2978,
2968,2940,2920,2912,2904,2890,2876,2867,
2857,2848,2837,2827,2816,2807,2797,2788,
2780,2772,2764,2756,2742,2728,2722,2717,
2711,2706,2700,2692,2684,2676,2668,2661,
2654,2647,2640,2633,2626,2618,2611,2604,
2598,2592,2586,2580,2572,2564,2558,2552,
2544,2536,2532,2528,2520,2518,2516,2512,
2508,2504,2500,2496,2492,2488,2484,2480,
2477,2474,2471,2468,2466,2465,2463,2461,
2459,2458,2456,2455,2454,2453,2452,2452,
2452,2452,2452,2464,2464,2444,2438,2432,
2428,2424,2417,2411,2404,2396,2388,2376,
2364,2353,2343,2332,2316,2300,2289,2278,
2267,2256,2244,2232,2222,2212,2192,2184,
2156,2148,2128,2120,2116,2108,2104,2100,
2096,2052,2048,2048,2044,2044,2040,2040,
2040,2040,2040,2040,2040,2040,2056,2048,
2046,2044,2042,2040,2038,2036,2034,2032,
2030,2028,2028,2026,2024,2020,2020,2020,
2016,2012,2012,2008,2004,2004,2002,2000,
1992,1936,1932,1928,1924,1920,1916,1912,
1910,1907,1905,1902,1900,1896,1892,1892,
1892,1892,1892,1892,1900,1904,1908,1912,
1920,1928,1932,1930,1928,1925,1923,1920,
1912,1904,1892,1882,1848,1836,1824,1784,
1780,1776,1768,1760,1756,1748,1740,1732,
1728,1722,1716,1712,1708,1704,1700,1696,
1688,1684,1680,1676,1672,1668,1666,1660,
1656,1648,1646,1644,1640,1636,1632,1628,
1628,1624,1620,1616,1614,1612,1612,1610,
1608,1608,1606,1604,1603,1601,1600,1600,
1600,1600,1600,1600,1600,1598,1596,1596,
1596,1596,1596,1596,1596,1596,1596,1596,
1596,1596,1596,1596,1596,1596,1596,1596,
1596,1596,1596,1592,1588,1582,1576,1570,
1564,1548,1543,1537,1532,1516,1508,1504,
1500,1496,1492,1488,1484,1480,1476,1468,
1466,1464,1460,1456,1452,1450,1448,1440,
1437,1435,1432,1424,1421,1419,1416,1414,
1412,1408,1406,1404,1404,1400,1396,1392,
1392,1388,1384,1380,1376,1372,1368,1364,
1363,1361,1360,1352,1349,1347,1344,1342,
1340,1336,1332,1328,1324,1322,1320,1318,
1316,1313,1311,1308,1306,1304,1304,1304,
1300,1299,1297,1296,1296,1296,1296,1296,
1296,1296,1295,1293,1292,1288,1286,1284,
1284,1284,1246,1244,1243,1241,1240,1238,
1236,1234,1232,1230,1228,1227,1225,1224,
1220,1216,1214,1212,1208,1204,1200,1196,
1192,1188,1184,1180,1176,1172,1168,1164,
1160,1156,1152,1148,1143,1137,1132,1128,
1124,1119,1113,1108,1100,1092,1082,1072,
1067,1061,1056,1040,1028,1012,1002,992,
984,976,968,956,940,928,916,904,
868,836,824,432,420,380,372,346,
336,316,272,244,208,128,112,104,
96,80,70,60,53,47,40,30,
20,14,8,2,2,2,2,0};


const uint16_t profile5[512] = { 
4080,4080,4080,4076,4076,2064,2064,2064,
2060,2060,2060,2058,2056,2056,2056,2052,
2052,2052,2052,2052,2052,2052,2052,2052,
2052,2052,2052,2052,2054,2056,2058,2060,
2064,2068,2074,2080,2085,2091,2096,2100,
2104,2109,2115,2120,2124,2128,2136,2142,
2148,2152,2158,2164,2168,2172,2176,2184,
2188,2192,2196,2200,2204,2220,2224,2228,
2234,2240,2248,2256,2264,2272,2276,2284,
2284,2284,2284,2284,2284,2284,2284,2284,
2283,2283,2282,2281,2281,2280,2279,2278,
2277,2276,2274,2273,2271,2270,2268,2267,
2266,2265,2264,2263,2261,2260,2258,2256,
2254,2252,2248,2244,2240,2240,2236,2232,
2228,2228,2226,2224,2220,2216,2212,2208,
2208,2200,2198,2196,2192,2184,2180,2180,
2180,2176,2172,2168,2164,2160,2160,2160,
2156,2152,2148,2148,2144,2144,2142,2140,
2136,2136,2136,2134,2132,2131,2129,2128,
2126,2125,2123,2122,2120,2120,2120,2120,
2120,2118,2116,2114,2112,2112,2112,2111,
2109,2108,2108,2108,2106,2104,2104,2104,
2104,2104,2104,2104,2104,2104,2104,2104,
2104,2104,2104,2104,2104,2104,2104,2104,
2104,2104,2104,2104,2104,2104,2104,2100,
2100,2100,2100,2098,2096,2096,2096,2096,
2096,2096,2095,2093,2092,2092,2092,2092,
2090,2088,2088,2087,2085,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2080,
2080,2080,2080,2080,2080,2080,2080,2076,
2076,2076,2076,2076,2076,2076,2076,2075,
2073,2072,2071,2069,2068,2067,2066,2066,
2065,2064,2063,2062,2061,2060,2060,2060,
2060,2060,2058,2056,2054,2052,2052,2052,
2052,2052,2052,2052,2052,2052,2052,2052,
2052,2052,2052,2052,2052,2052,2052,2052,
2052,2052,2052,2052,2052,2052,2052,2052,
2052,2052,2052,2052,2052,2052,2052,2050,
2048,2048,2048,2048,2048,2048,2048,2048,
2048,2048,2048,2048,2048,2048,2048,2048,
2048,2048,2048,2048,2048,2048,2048,2048,
2048,2048,2048,2048,2048,2048,2048,2048,
2048,2048,2048,2048,2048,2048,2048,2052,
2052,2052,2052,2052,2052,2056,2056,2056,
2056,2056,2056,2060,2060,2060,2060,2062,
2064,2064,2064,2064,2068,2068,2068,2068,
2068,2072,2072,2072,2072,2072,2072,2072,
2072,2072,2072,2072,2074,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2076,2076,2076,2076,2076,
2076,2076,2076,2078,2080,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
2084,2084,2084,2084,2084,2084,2084,2084,
0,0,0,0,0,0,0,0};











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
int env_duty;
int a;
int movement;
//uint16_t h;
int breaker = 1;
int rate;
int state = 0;
int actdelay;
float dong;
bool wrong = false;
int Adel;
int deep;
int profilearray[12];
bool trigger = false;
bool flip = false;
int shift;
int timer1_counter;
float gate;
int attack;
int val;
float depth;
int centre;
float sens;
int hate;
int Threshold = 500;
float AUDIN;
float THELISTENER;
float LARGER;
int RAWIN;
int offset = 0; //adjust to allow for bias of audio input 
int maxVal = 0;
float OUT1;
float OUT2;
float OUT3;
float OUT4;
int  pro_env_more;
float env_sens;
float env;
//-->DECLARE  THINGS
int timeo = 0;
#define DC_OFFSET  0  // DC offset in mic signal - if unsure, leave 0
#define NOISE     10 // Noise/hum/interference in mic signal
#define SAMPLES   60  // Length of buffer for dynamic level adjustment
#define TOP       10 // Allow dot to go slightly off scale
int
vol[SAMPLES],       // Collection of prior volume samples
lvl       = 10,      // Current "dampened" audio level
minLvlAvg = 0,      // For dynamic adjustment of graph low & high
maxLvlAvg = 4096;
int value=0;
int maxAudioValue = 0;          // Maximum analog value read from audio channel
int ledLevel;
//const int ledCount = 255;
const int numReadings = 20;
int readings[numReadings];
boolean showPeak = false;
int peakValue = 0;
long lastPeak = 0;


const int ENVA_NUMSAMPLE = 1005; //Samples taken. More = Smoother but also slower.
//sample variables for smoothing (leave these alone)
int ENVA_SAMP[ENVA_NUMSAMPLE] = {0}; //value of sample
int ENVA_INDEX = 0; //number of sample from 1-[NUMSAMPLE]
int ENVA_TOTAL = 0; //running total of sample values
int envattack = 100;


float ENV_REL;
float ENV_ATT;

int profile_duty;
int pal;
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
int pmax = 0;
int profile_avg;
int profile_triggered;
int MASTER_PROFILE;

int selection[5] = {0,0,0,0,0}; 
float Master;

//// main compressor parameters. Adjust these to your needs. ////
//int attack_f;  // attack period (how soon the compressor will start attenuating loud signals) given in measurement frame
                    // units (see window_ms). Default setting corresponds to 50ms. Max buf_len / 2. Min 4.
int release_f; // release period (how soon the compressor will soften attenuation after signals have become more silent),
                    // given in measurement frame units. Default setting corresponds to 200ms; Max buf_len.
                    // Does not have an effect if <= attack_f
int threshold = 50; // minimum signal amplitude before the compressor will kick in. Each unit corresponds to roughly 5mV
                    // peak-to-peak.
float ratio = 10.0;  //was 2.0 dampening applied to signals exceeding the threshold. n corresponds to limiting the signal to a level of
                    // threshold level plus 1/3 of the level in excess of the threshold (if possible: see duty_min, below)
                    // 1(min) = no attenuation; 20(max), essentially limit to threshold, aggressively
const float max_transition_rate = 1.11; // although the moving averages for attack and release will result in smooth transitions
                    // of the compression rate in most regular cases sudden signal spikes can result in abrupt transitions, introducing
                    // additional artefacts. This limits the maximum speed of the transition to +/- 11% of current value.

//// Some further constants that you will probably not have to tweak ////
#define DEBUG 1           // serial communication appears to introduce audible noise ("ticks"), thus debugging is diabled by default
const int window_ms = 5;  // milliseconds per measurement window. A narrow window will allow finer control over attack and release,
                    // but it will also cripple detection of low frequency amplitudes. Probably you don't want to change this.
const int buf_len = 100;  // size of buffer. attack_f and release_f cannot exceed this.
const int duty_min = 10;  // ceiling value for attenuation (lower values = more attenuation, 0 = off, 255 = no attenuation)
                    // beyond a certain value further attenuation is just too coarse grained for good results. Ideally, this
                    // value is never reached, but might be for aggressive dampening ratio and low thresholds.
const int duty_warn = 2 * duty_min;  // See above. At attenuation beyond this (i.e. smaller numbers), warning LED will flash.
                    // Reaching this point on occasion is quite benign. Reaching this point much of the time means too strong
                    // signal, too low threshold setting, or too aggressive inv_ratio.
const int signal_warn = 300;  // A warning LED will flash for signals exceeding this amplitude (5mv per unit, peak-to-peak) as
                    // it is probably (almost) too much for the circuit too handle (default value corresponds to about +-750mV
                    // in order to stay below the .8V typical 2N7000 body diode forward voltage, as well as below
                    // the 1.7V signal swing (centered at 3.3V) that the Arduino can handle).


//// working variables ////
volatile int cmin = 1024; // minimum amplitude found in current measurement window
volatile int cmax = 0;    // maximum amplitude found in current measurement window
int buf[buf_len];         // ring buffer for moving averages / sums
int pos = 0;              // current buffer position
int attack_mova = 0;      // moving average (actually sum) of amplitudes over past attack period
int release_mova = 0;     // moving average (actually sum) of amplitudes over past release period
int32_t now = 0;          // start time of current loop
int32_t last = 0;         // time of last loop
int duty = 3000;           // current PWM duty cycle for attenuator switch(es) (0: hard off, 255: no attenuation)
byte display_hold = 0;
float invratio = 1 / ratio;  // inverse of ratio. Saves some floating point divisions



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
  //pinMode(PA4, INPUT);//POTS
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



  for (int i = 0; i < buf_len; ++i) {  // clear buffer
    buf[i] = 0;
  }


one();
last = millis();

 
}

void loop() {
 
  uint8_t  i;
  uint16_t minLvl, maxLvl;
  int n;
  n   = val;
  //n  = abs(n - 512 - DC_OFFSET); // Center on zero
  n  = (n <= NOISE) ? 0 : (n - NOISE);     // Remove noise/hum
  lvl = ((lvl * 7) + n) >> 3;    // "Dampened" reading (else looks twitchy)


  //value = analogRead(A1);
  if (n > maxAudioValue){
  maxAudioValue = n; 
  }


  ledLevel = map(n, 0, 2048, 0, 4096);
 

  if (millis() > (lastPeak + 2000))
  { 
  peakValue = peakValue + (release_f);
  
  }
  if (ledLevel > peakValue)
  {
  peakValue = ledLevel;
  lastPeak = millis();

  }

//Circular - Foward and Back with Envelope.
profile_duty =((0-(duty - 1000)/2)+500);
//Long Average
profile_avg =  env/5;
//Trigger once 
if (profile_duty >= pmax){pmax = profile_duty;}
profile_triggered = pmax;
env_duty =  peakValue;


GET_PROFILES();


if (profilearray[0] == 1){FREQ_PROFILE = 0-profilearray[8];}else{FREQ_PROFILE =profilearray[8]; }


Master = (profilearray[9]);


OUT1 = (profilearray[2] + (profilearray[3]*depth))*Master; //
OUT2 = (profilearray[4] + (profilearray[5]*depth))*Master; 
OUT3 = (profilearray[6] + (profilearray[7]*depth))*Master; 
OUT4 = centre  + (FREQ_PROFILE);
//Master = (profilearrayprofilearray[3][9]*depth);

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

//duty = movement (2000) - 1000
//env (0-2500ish (in theory 4096))
//profile_duty (0-500)
//profile_avg (0-500)
//profile_triggered (0-500)
//env_duty = (0-movement (2000))

Adel  = env_duty;
if (Adel  >= 100){
  digitalWrite(PB5, HIGH);
}else{digitalWrite(PB5, LOW);}

if (Adel  >= 200){
  digitalWrite(PB4, HIGH);
}else{digitalWrite(PB4, LOW);}


if (Adel   >= 300){
  digitalWrite(PA11, HIGH);
}else{digitalWrite(PA11, LOW);}


if (Adel  >= 400){
  digitalWrite(PA10, HIGH);
}else{digitalWrite(PA10, LOW);}

if (Adel  >= 500){
  digitalWrite(PA9, HIGH);
}else{digitalWrite(PA9, LOW);}









}





///ALL OTHER BORING STUFF


void handler(void) {
 checkthings();
}; 





void checkthings(){

/*RELEASE */
//release_f = analogRead(PA2); // change to relase / smoothing
//release_f = map(release_f, 0, 4095, -20, 50);
/*ATTACK */
//attack_f = analogRead(PA4); // attack
//attack_f = map(attack_f, 0, 4095, 10, 100);




/*DEPTH */
release_f = analogRead(PA3); // depth
release_f = map(release_f, 0, 4095, 1, 100);
//depth = depth/100;

/*SENSITIVITY */
sens = analogRead(PA1); // chanege to sens
movement= map(sens, 0, 4095, 50, 2000);


/*CENTRE FREQUENCY */
centre = analogRead(PA5); //centre freq    
centre = map(centre, 0, 4095, profilearray[11], profilearray[10]);
/*AUDIO IN */
val = analogRead(PA0); // AUDIOI
//val = map( pal , 0, 4095, 0,1024);
//Here is the start of the smoothe


}

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
  void CHANGEPRESET() {=
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
//duty = movement (2000) - 1000
//env (0-2500ish (in theory 4096))
//profile_duty (0-500)
//profile_avg (0-500)
//profile_triggered (0-500)
//env_duty = (0-movement (2000))
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 2096; //HP CENTRE VOLUME
profilearray[3]  = env;  //  HP Profile Variable or number

profilearray[4]  = 4096; // BP CENTRE VOLUME
profilearray[5]  = 0; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = -profile2[profile_avg];  // LP Profile Variable or number

profilearray[8]  = env_duty;//;(env_duty+env)/2; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[9]  = 1; //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob*4
profilearray[11]  = 2047; //lower scope of centre frequency knob
      break;

//profilearray[1,1.0,4095,]        
    

    case 2:
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 50; // envelope depth as percentage

profilearray[2]  = 2000; //HP CENTRE VOLUME
profilearray[3]  = -env_duty/2;  //  HP Profile Variable or number

profilearray[4]  = 2000; // BP CENTRE VOLUME
profilearray[5]  = -env_duty/2; //BP Profile Variable or number

profilearray[6]  = 2000; // LP CENTRE VOLUME
profilearray[7]  = -env_duty/2;  // LP Profile Variable or number

profilearray[8]  = 0;//;(env_duty+env)/2; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[9]  = 1; //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob*4
profilearray[11]  = 2047; //lower scope of centre frequency knob
      break;


    case 3:
      /* PROFILE INFO */
//put into array?
//put into array?
Q(3); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 0; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 4096; //HP CENTRE VOLUME
profilearray[3]  = -LARGER;  //  HP Profile Variable or number

profilearray[4]  = 4096; // BP CENTRE VOLUME
profilearray[5]  = (-env)+profile4[pro_env]; //BP Profile Variable or number

profilearray[6]  = 4096; // LP CENTRE VOLUME
profilearray[7]  = 0;  // LP Profile Variable or number

profilearray[8]  = -env; //BP Profile Variable or number; // Freq Profile, Variable or number
profilearray[9]  = (profile5[pro_env*4]); //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
      break;

    case 4:     
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 0; // envelope up or down
profilearray[1]  = 100; // envelope depth as percentage

profilearray[2]  = 4096; //HP CENTRE VOLUME
profilearray[3]  = (-profile4[pro_env]/4); //  HP Profile Variable or number

profilearray[4]  = 0; // BP CENTRE VOLUME
profilearray[5]  = 0; //BP Profile Variable or number

profilearray[6]  = 0; // LP CENTRE VOLUME
profilearray[7]  = profile4[pro_env]; // LP Profile Variable or number

profilearray[8]  = env; // Freq Profile, Variable or number
profilearray[9]  = (profile5[pro_env]-2048); //Profile of Master

profilearray[10]  = 4096; //higher scope of centre frequency knob
profilearray[11]  = 0; //lower scope of centre frequency knob
        /* PROFILE INFO */
      break;

    case 5:
      /* PROFILE INFO */
//put into array?
Q(1); //1, 3, 4, 2; 1 is stronger
profilearray[0] = 1; // envelope up or down
profilearray[1]  = 1.0; // envelope depth as decimal
profilearray[2]  = profile1[pro_env]-2048; //  HP Profile Variable or number
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
profilearray[2]  = profile1[pro_env]-2048; //  HP Profile Variable or number
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
