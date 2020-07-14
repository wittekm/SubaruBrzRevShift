#pragma once
#include "deps.hpp"
#include "interfaces/i_obd_data_provider.hpp"

class TestObdDataProvider : public IObdDataProvider
{
private:
    Deps &deps;

public:
TestObdDataProvider(Deps &deps) : deps(deps) {}
    int getRpm()
    {
        return (deps.curTimeMillis() / 10) % 4000;
    }

    float getSpeedMph()
    {
        return (deps.curTimeMillis() / 1000) % 60;
    }
};
