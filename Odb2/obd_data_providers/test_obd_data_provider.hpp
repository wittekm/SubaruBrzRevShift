#pragma once
#include "../deps.hpp"
#include "../logic/evts.hpp"
#include "../interfaces/i_obd_data_provider.hpp"

class TestObdDataProvider : public IObdDataProvider
{
private:
    Deps &deps;

public:
TestObdDataProvider(Deps &deps) : deps(deps) {}
    optional<int> getRpm()
    {
        return (deps.curTimeMillis() / 100 * 10) % 900 * 10;
    }

    optional<float> getSpeedKph()
    {
        return (deps.curTimeMillis() / 100 ) % 120;
    }
};

// change urself w keyboard
class ManualObdDataProvider : public IObdDataProvider
{
private:
    Deps &deps;
    int rpm;
    float kph;

public:
ManualObdDataProvider(Deps &deps) : deps(deps) {}
    optional<int> getRpm()
    {
        return rpm;
    }

    optional<float> getSpeedKph()
    {
        return kph;
    }

    void handleKbEvent(const Event event) override {
        switch (event.payload)
        {
        case 'r': {
            rpm = (rpm + 1000) % 9000;
        }
        break;

        case 'k': {
            // lmfao ok max
            kph = float( (int(kph) + 10) % 120);
        }
        break;

        default:
            break;
        }

    }

};