#pragma once
#include "../deps.hpp"
#include "../interfaces/i_obd_data_provider.hpp"

class TestObdDataProvider : public IObdDataProvider
{
private:
    Deps &deps;

public:
TestObdDataProvider(Deps &deps) : deps(deps) {}
    int getRpm()
    {
        return (deps.curTimeMillis() / 100 * 10) % 900 * 10;
    }

    float getSpeedKph()
    {
        return (deps.curTimeMillis() / 100 ) % 120;
    }
};
