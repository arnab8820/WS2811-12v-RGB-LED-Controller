#include <LittleFS.h>
#include <ArduinoJson.h>
#include "../shared_vars.h"

void initFs(){
    if (!LittleFS.begin()){
        Serial.println("Error mounting file system");
        return;
    }
}

void saveConfig(){
    File configFile = LittleFS.open("/config.txt", "w");
    if(!configFile){
        Serial.println("Error openning the file");
        return;
    }
    DynamicJsonDocument configObj(1024);
    configObj["brightness"] = brightness;
    configObj["color"]["red"] = red;
    configObj["color"]["green"] = green;
    configObj["color"]["blue"] = blue;
    configObj["fx"]["id"] = ledFxId;
    configObj["fx"]["speed"] = fxSpeed;

    String config;
    serializeJson(configObj, config);
    configFile.print(config);
    delay(10);
    configFile.close();
}

void readConfig(){
    File configFile = LittleFS.open("/config.txt", "r");
    if(!configFile){
        Serial.println("Error openning the file");
        return;
    }
    String data;
    while (configFile.available()){
        data += configFile.readString();
    }
    configFile.close();
    DynamicJsonDocument configObj(1024);
    deserializeJson(configObj, data);
    brightness = configObj["brightness"];
    red = configObj["color"]["red"];
    green = configObj["color"]["green"];
    blue = configObj["color"]["blue"];
    ledFxId = configObj["fx"]["id"];
    fxSpeed = configObj["fx"]["speed"];
    if(ledFxId == 0){
        colorChanged = true;
    }
}