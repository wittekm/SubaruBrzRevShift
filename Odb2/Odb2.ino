#include <M5Stack.h>
#include "deps.hpp"
#include "faces.hpp"
#include "odb2_renderer.hpp"
#include "test_obd_data_provider.hpp"

// System-wide objects
Deps deps;
KeyboardFace kf(deps);
IScene * currScene = NULL;

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  M5.Lcd.setTextSize(2);
    
  // LCD display
  M5.Lcd.println("A for off, B for OBD, C for OBD setup");
}


IObdDataProvider * dataProvider = new TestObdDataProvider(deps);

bool bPressed = false;

// the loop routine runs over and over again forever
void loop() {

  kf.loop();
  
  // shutdown
  if(M5.BtnA.wasPressed()) {
    M5.Lcd.println("ok buhbye");
    delay(1000);
    M5.powerOFF();
  }

  // turn on currScene->loop
  if(M5.BtnB.wasPressed()) {
    bPressed = true;
  }

  // setup
  if(M5.BtnC.wasPressed()) {
    M5.Lcd.println("setting up obd...");
    // TODO real data provider instead of fake
    currScene = new ODB2IrlScene(
      dataProvider, deps
    );
    currScene->setup();
  }
  
  if(bPressed) {
    currScene->loop();
  }
  M5.update();
  deps.sleep(200);
}
