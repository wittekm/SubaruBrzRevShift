#include "BluetoothSerial.h"
#include "ELMduino.h"

#define PRINT_PORT M5.Lcd
#define DEBUG_PORT Serial
#define ELM_PORT   SerialBT
int LED_BUILTIN = 2;

BluetoothSerial SerialBT;
ELM327 myELM327;
uint32_t rpm = 0;


void setupOdb()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  DEBUG_PORT.begin(115200);
  //ELM_PORT.setPin("1234");
  ELM_PORT.begin("ESP32test", true);

  PRINT_PORT.println("Attempting to connect to ELM327...");

  if (!ELM_PORT.connect("OBDII"))
  {
    PRINT_PORT.println("Couldn't connect to OBD scanner, phase1");
    while(1);
  }

  if (!myELM327.begin(ELM_PORT))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  PRINT_PORT.println("Connected to ELM327");
}


void odbLoop()
{
  float tempRPM = myELM327.rpm();

  if (myELM327.status == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    PRINT_PORT.clear(BLACK);
    PRINT_PORT.setCursor(0, 0);
    PRINT_PORT.print("RPM: "); PRINT_PORT.println(rpm);
  }
  else {
    PRINT_PORT.println("fail");
  }
}
