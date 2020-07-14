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

static float TIRE_RADIUS_CM = 62.738 / 2;
static float TIRE_CIRCUMFERENCE_M = 2.0 * PI * TIRE_RADIUS_CM / 1000.0;

static float FINAL_RATIO = 4.10;


float rpmAtGear(float speed, int gear) {
    // mph * (gear * final) / circ = rpm
    float expectedRpm = float(speed) * (GEAR_RATIOS[gear] * FINAL_RATIO) / TIRE_CIRCUMFERENCE_M;
    return expectedRpm;
}

float speedAtGear(float rpm, int gear) {
    // this is in kph, but the formula is:
    // mph = (rpm * cir) / (gear * final) 
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