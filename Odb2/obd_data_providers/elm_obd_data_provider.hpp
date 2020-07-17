#pragma once

#include "../interfaces/i_obd_data_provider.hpp"
#include "../deps.hpp"

#include "BluetoothSerial.h"
#include "ELMduino.h"

#include <experimental/optional>
using namespace std::experimental;

#define DEBUG_PORT Serial
#define ELM_PORT   SerialBT

BluetoothSerial SerialBT;

class ElmObdDataProvider : public IObdDataProvider {
   private:
      Deps& deps;
      ELM327 elm;
      uint32_t rpm;
      boolean valid; // gross! but no exceptions

      ElmObdDataProvider(Deps& deps): deps(deps)
      {
         //DEBUG_PORT.begin(115200);
         ELM_PORT.begin("ESP32test", true);
         deps.lcd.setCursor(0,0);
         deps.lcd.println("Attempting to connect to ELM327...");

         if (!ELM_PORT.connect("OBDII"))
         {
            deps.lcd.println("Couldn't connect to OBD scanner, phase1");
            return;
         }

         if (!this->elm.begin(ELM_PORT))
         {
            deps.lcd.println("Couldn't connect to OBD scanner - Phase 2");
            return;
         }

         deps.lcd.println("Connected to ELM327");
         valid = true;
      }

   public:
      static optional<ElmObdDataProvider*> get(Deps& deps) {
         auto provider = new ElmObdDataProvider(deps);
         if (provider->valid) {
            return optional<ElmObdDataProvider*>(provider);
         } else {
            return nullopt;
         }
      }

      optional<int> getRpm()
      {
         int value = elm.rpm();
         return elm.status == ELM_SUCCESS ? optional<int>(value) : nullopt;
      }

      optional<float> getSpeedKph()
      {
         float value = elm.kph();
         return elm.status == ELM_SUCCESS ? optional<float>(value) : nullopt;
      }
};
