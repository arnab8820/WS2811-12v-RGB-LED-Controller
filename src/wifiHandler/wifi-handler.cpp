#include <ESP8266WiFi.h>
#include "../shared_vars.h"

#define WIFICONTIMEOUT 240

const char* ssid     = "Arnab-Home";
const char* password = "arnabhome";

const char* apSsid = "Matrix Clock";
const char* apPasswd = "";

void startAp(){
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  delay(1000);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void initWifi(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  // WiFi.hostname(deviceName);
  WiFi.begin(ssid, password);
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount <= WIFICONTIMEOUT) {
    retryCount++;
    delay(500);
    Serial.print(".");    
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect WiFi");
    startAp();
  }
  
}

bool isWifiConnected(){
  return WiFi.status() == WL_CONNECTED;
}