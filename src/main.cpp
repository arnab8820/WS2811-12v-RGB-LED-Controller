#include <Arduino.h>

// project source file includes
#include "shared_vars.h"
#include "ledHandler/led_driver.h"
#include "wifiHandler/wifi-handler.h"
#include "otaHandler/ota-handler.h"
#include "apihandler/api-handler.h"
#include "fileSystem/fsHandler.h"
#include "ledFx/fx.h"

const char* deviceName = "Desk RGB Led";

// global vars
int red = 0;
int blue = 0;
int green = 0;
int brightness = 100;
int ledFxId = 1;
int fxSpeed = 9;

// flag vars
bool colorChanged = false;


void setup() {
  Serial.begin(115200);
  //init led controller
  initLed();
  red = 255;
  blue = 0;
  green = 0;
  setLedColor();
  initFs();
  readConfig();

  //init wifi
  initWifi();

  //init ota
  initOta();

  //register and start web server
  registerWebServer();
  startWebServer();
  delay(1000);
}

void loop() {
  otaHandler();
  handleWebTraffic();
  handleFx();
}