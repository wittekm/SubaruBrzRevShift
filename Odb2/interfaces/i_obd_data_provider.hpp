#pragma once
#include <experimental/optional>
using namespace std::experimental;

class IObdDataProvider {
   public:
      // pure virtual function providing interface framework.
      virtual optional<int> getRpm() = 0;
      virtual optional<float> getSpeedKph() = 0;
};
