#ifndef ARDUINO

#include "gear_find.hpp"
#include <iostream>
using namespace std;

int main()
{
    /**
     *  RPM: 7800
     *  Final Drive Ratio: 4.100
        1st Gear (kmh): 61
        2nd Gear (kmh): 102
        */
    //float gearMatch = closestGearMatch(102, 7800);
    float gearMatch = 1;
    float rpmExpected = 0;//rpmAtGear(102, 2); // it should be 7800...

    float expect62 = speedAtGear(7800, 2);
    cout << "Expected 62" << expect62 << endl;

    cout << "Best gear guess (should be gear2)" << gearMatch << endl 
        << "rpm expected (ideally 7800)" << rpmExpected << endl;
    return 0;
}
#endif