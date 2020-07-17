#pragma once
#include "../interfaces/i_scene.hpp"
#include "../interfaces/i_obd_data_provider.hpp"
#include "../deps.hpp"
#include "../logic/gear_find.hpp"
#include "frame_preserver.hpp"

#define PI 3.1415926535897932384626433832795

#define DEG_TO_RAD 0.017453292519943295769236907684886

static float RADIUS = 50;
static int MID_X = 320 / 2 + 100;
static int MID_Y = 240 / 2 - 25;

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
        MID_X, MID_Y, 5, BRZ_ORANGE);

    deps.lcd.drawCircle(
        MID_X, MID_Y, RADIUS, BRZ_ORANGE);

    auto draw = [&](float angleDegs, int color)
    {
        float angleRads = toRads(angleDegs);
        // draw the bottom and the top
        deps.lcd.drawLine(
            MID_X, MID_Y,
            MID_X + sin(angleRads) * RADIUS,
            MID_Y + cos(angleRads) * RADIUS,
            color);
    };

    draw(MIN_DEGS, YELLOW);
    draw(MAX_DEGS, YELLOW);
    draw(angleDegs, BRZ_ORANGE);
}

class ODB2IrlScene : public IScene
{
private:
    IObdDataProvider *dataProvider;
    Deps &deps;

    FramePreserver downshiftSuggestion;

public:
    ODB2IrlScene(
        IObdDataProvider *dataProvider,
        Deps &deps) : dataProvider(dataProvider),
                      deps(deps),
                      downshiftSuggestion(5)
    {
    }

    void setup()
    {
        deps.lcd.clear(BLACK);
    }

    void loop()
    {
        optional<int> rpm = dataProvider->getRpm();
        optional<float> speed = dataProvider->getSpeedKph();

        deps.lcd.setTextSize(4);
        deps.lcd.setTextColor(YELLOW, BLACK);
        deps.lcd.setCursor(0, 0);
        if (rpm && speed)
        {
            //deps.lcd.fillScreen(BLACK);
            deps.lcd.print("RPM: ");
            deps.lcd.println(rpm.value_or(-1));

            deps.lcd.print("KPH: ");
            deps.lcd.println(speed.value_or(-1));

            int gearMatch = 0;
            if (rpm && speed)
            {
                gearMatch = closestGearMatch(*speed, *rpm);
                deps.lcd.print("G?: ");
                deps.lcd.println(gearMatch);
            }

            deps.lcd.setCursor(0, 240 - 60);
            optional<int> newDownshiftSuggestion = rpmAtGear(*speed, gearMatch - 1);
            if (newDownshiftSuggestion)
            {
                if (downshiftSuggestion.incrFrame(*newDownshiftSuggestion))
                {
                    String s = String(downshiftSuggestion.value);
                    String asdf = String(s + "    ");
                    deps.text(
                        asdf.c_str(),
                        6, BRZ_ORANGE);
                    drawAngleBRZ(deps, *rpm);
                }
                else
                {
                    deps.text("---            ", 6);
                }
            }
            else
            {
                deps.lcd.println("rpm 0 wtf");
            }
        }
    }
};
