#define ESP32
#include <ELMduino.h>
#include <M5Stack.h>
#include <experimental/optional>
#include "deps.hpp"
#include "faces.hpp"
#include "scenes/odb2_renderer.hpp"
#include "obd_data_providers/test_obd_data_provider.hpp"
#include "obd_data_providers/elm_obd_data_provider.hpp"

using namespace std::experimental;

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
  deps.text("BTN: A->off, C->setup", 2);
  //deps.lcd.println("A for off\nB for OBD\nC for OBD setup");
  deps.text("KB: T->test, I->irl\nM->manual", 2);
  //deps.lcd.println("T for test\nI for IRL");
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
    handleEvent(evt, state);
  }

  // turn on currScene->loop
  if (/*state.shouldSceneLoop && */state.currScene)
  {
    state.currScene->loop();
  }

  M5.update(); // read buttons
  framerateWait(deps, loopStartMillis);
}

static int DESIRED_FPS = 30;
static int TIME_BETWEEN_FRAMES = 1000 / DESIRED_FPS;

void framerateWait(Deps &deps, int loopStartMillis)
{
  int msForLoop = deps.curTimeMillis() - loopStartMillis;
  //assert msForLoop >= 0;
  int timeToWait = TIME_BETWEEN_FRAMES - msForLoop;
  if (timeToWait > 0)
  {
    deps.sleepMs(timeToWait);
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
  case 't': // test
    deps.lcd.println("Test");
    state.dataProvider = new TestObdDataProvider(deps);
    setScene(state, NULL);
    break;

  case 'm': // manual
    deps.lcd.println("Manual");
    state.dataProvider = new ManualObdDataProvider(deps);
    setScene(state, NULL);
    break;

  case 'c': // clear
    deps.freshScreen();
    break;

  case 'i':
  { // irl
    deps.freshScreen();
    deps.text("IRL");

    optional<ElmObdDataProvider *> provider = ElmObdDataProvider::get(deps);
    if (provider)
    {
      state.dataProvider = *provider;
    }
    else
    {
      deps.text("Couldn't build ElmObdDataProvider", 2, RED);
      deps.sleepMs();
    }
  }
  break;

  default:
    break;
  }

  if(state.dataProvider) {
    state.dataProvider->handleKbEvent(event);
  }
}

void setScene(State &state, IScene *scene)
{
  deps.lcd.println("New OBD2IrlScene");
  deps.sleepMs(1000);
  if(state.currScene) {
    delete state.currScene;
    state.currScene = NULL;
  }
  state.currScene = scene;
  if(state.currScene) {
    state.currScene->setup();
  }
}

void handleButtonEvent(const Event event, State &state)
{
  if (event.payload == 'a')
  {
    deps.freshScreen();
    deps.text("ok buhbye", 5, RED);
    deps.sleepMs();
    M5.powerOFF();
  }

  if (event.payload == 'b')
  {
    state.shouldSceneLoop = true;
  }

  if (event.payload == 'c')
  {
    deps.freshScreen();
    IScene * scene = new ODB2IrlScene(state.dataProvider, deps);
    setScene(state, scene);
  }
}