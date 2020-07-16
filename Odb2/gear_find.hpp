#pragma once
#define PI 3.1415926535897932384626433832795
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include <math.h>
#endif

// subaru brz
static float GEAR_RATIOS[] = {
    42.0, // dummy 0th gear
    3.626,
    2.188,
    1.541,
    1.213,
    1.000,
    0.767,
};

static float CM_TO_M = 1.0/100.0;
static float TIRE_DIAMETER_CM = 62.738;
static float TIRE_DIAMETER_M = 62.738 * CM_TO_M;
static float TIRE_RADIUS_CM = TIRE_DIAMETER_CM / 2.0;
static float TIRE_RADIUS_M = TIRE_RADIUS_CM * CM_TO_M;
static float TIRE_CIRCUMFERENCE_M = 2.0 * PI * TIRE_RADIUS_M;

static float FINAL_RATIO = 4.10;
static float MAGIC_CONSTANT = 336.13524;

/*
https://www.dropbox.com/s/o2ce1x7ddnhqueg/Screen%20Shot%202020-07-16%20at%203.18.34%20PM.png?dl=0
https://www.rcnmag.com/tech/calculating-mph-rpm-gear-ratio-tire-size
https://www.rcnmag.com/resources/images/RPMMPHGear-Ratio-1.jpg
336 magic constant
*/

float rpmAtGear(float speed, int gear) {
    // mph * (gear * final) / circ = rpm
    float expectedRpm = float(speed) * (GEAR_RATIOS[gear] * FINAL_RATIO) / TIRE_CIRCUMFERENCE_M;
    return expectedRpm;

    float numer = (speed * )
}

float speedAtGear(float rpm, int gear) {
    float expected = rpm * TIRE_DIAMETER_M / (GEAR_RATIOS[gear] * FINAL_RATIO * MAGIC_CONSTANT);
    return expected;
}

float speedAtGearOG(float rpm, int gear) {
    // this is in kph, but the formula is:
    // mph = (rpm * cir) / (gear * final) 
    //.... or so i thought
    /**
     * ans1 rpm
     * ans2 TIRE_DIAMETER_CM
     * ans3 rear gear ratio
     * ans4 final ratio
     * MPH = eval( Math.floor( ( (ans1 * ans2 ) / (ans3 * ans4 * 336.0) ) * 1000.0 + 0.5 ) / 1000.0 )
     *                                                                                             vv MPH to KMH 
     f.result2.value = ( ( (rpm * diameter_INCH ) / (gear * final * 336.0) ) * 1.609344 )
     */

    float expectedSpeed = float(rpm) * TIRE_CIRCUMFERENCE_M / (GEAR_RATIOS[gear] * FINAL_RATIO);
    return expectedSpeed;
}

int closestGearMatch(float speed, int rpm) {
    int bestMatchIdx = 0;
    float bestMatchScore = 10000;
    for (int i = 1; i <= 6; i++)
    {
        float expectedSpeed = speedAtGear(rpm, i);
        float score = abs(speed - expectedSpeed);
        if(score < bestMatchScore) {
            bestMatchScore = score;
            bestMatchIdx = i;
        }
    }
    return bestMatchIdx;
}