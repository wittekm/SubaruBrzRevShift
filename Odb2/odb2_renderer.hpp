#pragma once
#include "interfaces/i_scene.hpp"
#include "interfaces/i_obd_data_provider.hpp"
#include "deps.hpp"
#include "gear_find.hpp"

#define PI 3.1415926535897932384626433832795

#define DEG_TO_RAD 0.017453292519943295769236907684886

static float RADIUS = 75;
static int MID_X = 320 / 2 + 40;
static int MID_Y = 240 / 2 + 40;

// oh no, the 0 is at the bottom
static float MIN_DEGS = (240.0);
static float MAX_DEGS = (30.0);

// assuming its RGB565, gross
static int BRZ_ORANGE = 0xFB0B;

static float toRads(float degs)
{
    return (degs + 90.0) * DEG_TO_RAD;
}

static void
drawAngleBRZ(Deps &deps, int rpm)
{
    // https://www.automotiveaddicts.com/wp-content/uploads/2014/01/2014-subaru-brz-gauge-cluster.jpg

    // The result will be between -1 and 1.

    // min value    4pi/3      0rpm (235 deg-ish)
    // max value    pi/6    9000rpm (30 deg-ish)

    float totalArcDistance = (240.0 - 30.0);
    float percentOfGauge = float(rpm) / 9000.0;
    float angleDegs = MIN_DEGS - (float(rpm) / 9000 * totalArcDistance);
    float angleRads = toRads(angleDegs);

    // clear the angle area
    // drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    deps.lcd.fillRect(MID_X - RADIUS, MID_Y - RADIUS, RADIUS * 2, RADIUS * 2, BLACK);

    deps.lcd.fillCircle(
        MID_X, MID_Y, 5, BRZ_ORANGE
    );

    deps.lcd.drawCircle(
        MID_X, MID_Y, RADIUS, BRZ_ORANGE
    );

    deps.lcd.drawLine(
        MID_X, MID_Y,
        MID_X + sin(angleRads) * RADIUS,
        MID_Y + cos(angleRads) * RADIUS,
        BRZ_ORANGE);
}

class ODB2IrlScene : public IScene
{
private:
    IObdDataProvider *dataProvider;
    Deps &deps;

public:
    ODB2IrlScene(
        IObdDataProvider *dataProvider,
        Deps &deps) : dataProvider(dataProvider),
                      deps(deps)
    {
    }

    void setup()
    {
        deps.lcd.clear(BLACK);
    }

    void loop()
    {
        float rpm = dataProvider->getRpm();
        float speed = dataProvider->getSpeedKph();

        deps.lcd.setCursor(0, 0);
        if (rpm != 0)
        {
            //deps.lcd.fillScreen(BLACK);
            deps.lcd.print("RPM: ");
            deps.lcd.println(rpm);

            deps.lcd.print("KPH: ");
            deps.lcd.println(speed);

            deps.lcd.setCursor(0, 240-40);
            deps.lcd.print("GG: ");
            deps.lcd.println(closestGearMatch(speed, rpm));

            drawAngleBRZ(deps, rpm);
        }
        else
        {
            deps.lcd.println("fail");
        }
    }
};
