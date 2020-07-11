#include <M5Stack.h>

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

bool bPressed = false;

// the loop routine runs over and over again forever
void loop() {
  if(M5.BtnA.wasPressed()) {
    M5.Lcd.println("ok buhbye");
    delay(1000);
    M5.powerOFF();
  }
  if(M5.BtnB.wasPressed()) {
    bPressed = true;
  }
  if(M5.BtnC.wasPressed()) {
    M5.Lcd.println("setting up obd...");
    setupOdb();
  }
  
  if(bPressed) {
    M5.Lcd.println("odb time");
    odbLoop();
  }
  M5.update();
}
