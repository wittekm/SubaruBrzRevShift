#define ESP32
#include <ELMduino.h>
#include <M5Stack.h>
#include "deps.hpp"
#include "faces.hpp"
#include "odb2_renderer.hpp"
#include "test_obd_data_provider.hpp"
#include "elm_obd_data_provider.hpp"


struct State
{
  IScene *currScene = NULL;
  bool shouldSceneLoop = false;
  IObdDataProvider *dataProvider;
};
void handleButtonEvent(const Event event, State &state);
void handleKbEvent(const Event event, State &state);

// System-wide objects
Deps deps;
EventQueue eq;
State state;

// the setup routine runs once when M5Stack starts up
void setup()
{
  // Initialize the M5Stack object
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
  // get rid of annoying sound
  ledcDetachPin(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, INPUT);

  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(YELLOW, BLACK);

  // LCD display
  M5.Lcd.println("A for off\nB for OBD\nC for OBD setup");
  M5.Lcd.println("T for test I for IRL");
  kbSetup();
  state.dataProvider = new TestObdDataProvider(deps);
}



// the loop routine runs over and over again forever
void loop()
{
  const int loopStartMillis = deps.curTimeMillis();

  // Read inputs, add them as Events
  kbLoop(eq);
  btnLoop(eq);

  // Dispatch state-changing events
  Event evt = eq.get();
  if (!evt.isEmpty())
  {
    //deps.lcd.println("We got a live one");
    handleEvent(evt, state);
  }

  // turn on currScene->loop
  if (state.shouldSceneLoop && state.currScene)
    {
      state.currScene->loop();
    }

  M5.update(); // read buttons
  framerateWait(deps, loopStartMillis);
}

static int DESIRED_FPS = 30;
static int TIME_BETWEEN_FRAMES = 1000 / DESIRED_FPS;

void framerateWait(Deps& deps, int loopStartMillis) {
  int msForLoop = deps.curTimeMillis() - loopStartMillis;
  //assert msForLoop >= 0;
  int timeToWait = TIME_BETWEEN_FRAMES - msForLoop;
  if (timeToWait > 0) {
    deps.sleep(timeToWait);
  }
}

void handleEvent(const Event event, State &state)
{
  if (event.type == EventType::BTN)
  {
    handleButtonEvent(event, state);
  }
  else if (event.type == EventType::KB)
  {
    handleKbEvent(event, state);
  }
}

void handleKbEvent(const Event event, State &state)
{
  if (event.payload == 't')
  {
  }
  switch (event.payload)
  {
  case 't':
    deps.lcd.println("Test");
    state.dataProvider = new TestObdDataProvider(deps);
    break;

  case 'i':
    deps.lcd.println("IRL");
    state.dataProvider = new ElmObdDataProvider(deps);
    break;

  default:
    break;
  }
}

void handleButtonEvent(const Event event, State &state)
{
  if (event.payload == 'a')
  {
    M5.Lcd.println("ok buhbye");
    delay(1000);
    M5.powerOFF();
  }

  if (event.payload == 'b')
  {
    state.shouldSceneLoop = true;
  }

  if (event.payload == 'c')
  {
    M5.Lcd.println("setting up obd...");
    // TODO real data provider instead of fake
    state.currScene = new ODB2IrlScene(
        state.dataProvider, deps);
    state.currScene->setup();
  }
}