#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "../ledHandler/led_driver.h"
#include "../shared_vars.h"
#include "../fileSystem/fsHandler.h"
#include "../web-index.h"

#define index_html_gz_len 12263

ESP8266WebServer webServer(80);

void setCrossOrigin(){
    webServer.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    webServer.sendHeader(F("Access-Control-Max-Age"), F("600"));
    webServer.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
    webServer.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
};

// api endpoints definition
void handleRoot(){
    webServer.sendHeader(F("Content-Encoding"), F("gzip"));
    webServer.send_P(200, "Text/Html", (const char*)index_html_gz, index_html_gz_len);
}

void setColor(){
    String reqData = webServer.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reqData);
    red = doc["red"];
    green = doc["green"];
    blue = doc["blue"];
    colorChanged = true;
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Color set";
    resp["payload"]["color"]["red"] = red;
    resp["payload"]["color"]["green"] = green;
    resp["payload"]["color"]["blue"] = blue;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
    saveConfig();
}

void getColor(){
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Current color";
    resp["payload"]["color"]["red"] = red;
    resp["payload"]["color"]["green"] = green;
    resp["payload"]["color"]["blue"] = blue;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
}

void setBrightness(){
    String reqData = webServer.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reqData);    
    if(doc["brightness"]<0||doc["brightness"]>100){
        DynamicJsonDocument resp(1024);
        resp["code"] = 400;
        resp["payload"]["message"] = "Brightness must be between 0 and 100";
        resp["payload"]["brightness"] = doc["brightness"];
        String response;
        serializeJson(resp, response);
        webServer.send(200, "Application/json", response);
        return;
    }
    brightness = doc["brightness"];
    setLedColor();
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Brightness set";
    resp["payload"]["brightness"] = brightness;
    String response;
    serializeJson(resp, response);
    setCrossOrigin();
    webServer.send(200, "Application/json", response);
    saveConfig();
}

void getBrightness(){
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Current brightness";
    resp["payload"]["brightness"] = brightness;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
}

void getFxList(){
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "FX list";
    JsonArray fxList = resp["payload"].createNestedArray("list");
    for (int i = 0; i < 3; i++){
        DynamicJsonDocument fx(1024);
        fx["id"] = i;
        fx["name"] = ledFxList[i];
        fxList.add(fx);
    }
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
}

void getFx(){
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Current fx";
    resp["payload"]["fx"] = ledFxId;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
}

void setFx(){
    String reqData = webServer.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reqData);    
    ledFxId = doc["fx"];
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "fx set";
    resp["payload"]["fx"] = ledFxId;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
    saveConfig();
}

void getFxSpeed(){
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "Current speed";
    resp["payload"]["speed"] = fxSpeed;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
}

void setFxSpeed(){
    String reqData = webServer.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, reqData);   
    if(doc["speed"]<1||doc["speed"]>10){
        DynamicJsonDocument resp(1024);
        resp["code"] = 400;
        resp["payload"]["message"] = "Speed must be between 0 and 10";
        resp["payload"]["speed"] = doc["speed"];
        String response;
        serializeJson(resp, response);
        webServer.send(200, "Application/json", response);
        return;
    } 
    fxSpeed = doc["speed"];
    DynamicJsonDocument resp(1024);
    resp["code"] = 200;
    resp["payload"]["message"] = "speed set";
    resp["payload"]["speed"] = fxSpeed;
    String response;
    serializeJson(resp, response);
    webServer.send(200, "Application/json", response);
    saveConfig();
}

void handleCors(){
    setCrossOrigin();
    webServer.send(200, "Text/Plain", "");
}

// server handling functions
void registerWebServer(){
    webServer.on("/", handleRoot);
    webServer.on("/color/set", HTTP_OPTIONS, handleCors);
    webServer.on("/color/set", HTTP_POST, setColor);
    webServer.on("/color/get", HTTP_GET, getColor);
    webServer.on("/brightness/set", HTTP_OPTIONS, handleCors);
    webServer.on("/brightness/set", HTTP_POST, setBrightness);
    webServer.on("/brightness/get", HTTP_GET, getBrightness);
    webServer.on("/fx/list", HTTP_GET, getFxList);
    webServer.on("/fx/get", HTTP_GET, getFx);
    webServer.on("/fx/set", HTTP_OPTIONS, handleCors);
    webServer.on("/fx/set", HTTP_POST, setFx);
    webServer.on("/speed/get", HTTP_GET, getFxSpeed);
    webServer.on("/speed/set", HTTP_OPTIONS, handleCors);
    webServer.on("/speed/set", HTTP_POST, setFxSpeed);
}

void startWebServer(){
    webServer.begin();
}

void handleWebTraffic(){
    webServer.handleClient();
}
