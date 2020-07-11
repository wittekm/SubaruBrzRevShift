#pragma once

class IScene {
   public:
      // pure virtual function providing interface framework.
      virtual void setup() = 0;
      virtual void loop() = 0;
};
