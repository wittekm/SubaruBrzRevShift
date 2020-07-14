#pragma once

class IObdDataProvider {
   public:
      // pure virtual function providing interface framework.
      virtual int getRpm() = 0;
      virtual float getSpeedMph() = 0;
};
