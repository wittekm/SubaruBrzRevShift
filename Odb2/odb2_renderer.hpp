#pragma once
#include "interfaces/i_scene.hpp"
#include "interfaces/i_obd_data_provider.hpp"
#include "deps.hpp"


static void drawAngle(Deps &deps, int rpm)
{
    // The result will be between -1 and 1.
    int midX = 320 / 2;
    int midY = 240 / 2;

    float angleRads = float(rpm) / 400;

    float radius = 50;

    deps.lcd.drawLine(
        midX, midY,
        midX + sin(angleRads)*radius,
        midY + cos(angleRads)*radius,
        GREEN
    );
}

class ODB2IrlScene : public IScene
{
private:
    IObdDataProvider *dataProvider;
    Deps &deps;

public:
    ODB2IrlScene(
        IObdDataProvider *dataProvider,
        Deps &deps
    ): 
        dataProvider(dataProvider),
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

        if (rpm != 0)
        {
            deps.lcd.setCursor(0, 0);
            deps.lcd.print("RPM: ");
            deps.lcd.println(rpm);

            drawAngle(deps, rpm);
        }
        else
        {
            deps.lcd.println("fail");
        }
    }
};
