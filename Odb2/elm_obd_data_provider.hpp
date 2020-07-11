#pragma once

#include "interfaces/i_obd_data_provider.hpp"

#include "BluetoothSerial.h"
#include "ELMduino.h"

#define DEBUG_PORT Serial
#define ELM_PORT   SerialBT

BluetoothSerial SerialBT;

class ElmObdDataProvider : public IObdDataProvider {
   private:
      ELM327 elm;
      uint32_t rpm;

   public:
      ElmObdDataProvider()
      {
         DEBUG_PORT.begin(115200);
         ELM_PORT.begin("ESP32test", true);
         DEBUG_PORT.println("Attempting to connect to ELM327...");

         if (!ELM_PORT.connect("OBDII"))
         {
            DEBUG_PORT.println("Couldn't connect to OBD scanner, phase1");
            throw new std::bad_alloc("Couldn't connect to OBD scanner");
         }

         if (!this.elm.begin(ELM_PORT))
         {
            DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 2");
            throw new std::bad_alloc("Couldn't connect to OBD scanner pt2");
         }

         PRINT_PORT.println("Connected to ELM327");
      }

      int getRpm()
      {
         int tempRpm = elm.rpm();

         if (elm.status == ELM_SUCCESS)
         {
            return tempRpm;
         }
         else
         {
            // TODO: exception or golang style
            return 0
         }
      }
};
