#ifndef ARDUINO

#include "logic/gear_find.hpp"
#include <iostream>
using namespace std;

int main()
{
    /**
     *  RPM: 7800
     *  Final Drive Ratio: 4.100
            1st Gear (mph): 38
            2nd Gear (mph): 64

            1st Gear (kmh): 61
            2nd Gear (kmh): 102
        */
    float gearMatch = closestGearMatch(102, 7800);

    cout << "Speed at gear 2 @ 7800 - expected 102k: " << speedAtGear(7800, 2) << endl;
    cout << "RPM at gear 2 @ 62pmh - expected 7800ish: " << rpmAtGear(102, 2) << endl;

    cout << "Best gear guess (should be gear2)" << gearMatch << endl;
    return 0;
}
#endif