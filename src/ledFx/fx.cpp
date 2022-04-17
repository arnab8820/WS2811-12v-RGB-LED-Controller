#include <Arduino.h>

#include "../shared_vars.h"
#include "../ledHandler/led_driver.h"
#include "../fileSystem/fsHandler.h"
#include "../otaHandler/ota-handler.h"

String ledFxList[] = {"Flat", "Random", "Colorloop"};

long colorChangeTime=0;

int colorToLoop = 0;

void setRandomColor(){
  randomSeed(random(analogRead(A0)));
  red = random(256);
  randomSeed(random(analogRead(A0)));
  green = random(256);
  randomSeed(random(analogRead(A0)));
  blue = random(256);
  setLedColor();
  colorChangeTime = millis();
}

void handleFx(){
    switch (ledFxId){
        case 0: {
            // flat color
            if(!colorChanged){
                return;
            }
            setLedColor();
            colorChanged = false;
            break;
        }
        case 1: {
            //random color generation
            unsigned long timeout = map(fxSpeed, 1, 10, 300000, 5000);
            if(millis() - colorChangeTime >= timeout || colorChangeTime == 0){
                int tmpBrightness = brightness;
                for (int i = tmpBrightness; i >= 0; i--){
                    brightness = i;
                    setLedColor();
                    delay(map(tmpBrightness, 0, 100, 30, 10));
                }
                delay(100);
                otaHandler();
                setRandomColor();
                for (int i = 0; i <= tmpBrightness; i++){
                    brightness = i;
                    setLedColor();
                    delay(map(tmpBrightness, 0, 100, 30, 10));
                }
                brightness = tmpBrightness;
                saveConfig();
            }
            break;
        }
        case 2: {
            // color loop
            // unsigned long timeout = map(fxSpeed, 1, 10, 300000, 5000);
            // if(millis() - colorChangeTime >= timeout){

            // }
            break;
        }
        default: {
            Serial.println("Not a valid fx");
        }
    }
}