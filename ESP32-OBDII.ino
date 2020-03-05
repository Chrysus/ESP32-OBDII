#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

#include "BTClassicDevice.h"
#include "BTClassicManager.h"
#include "BTClassicDeviceLinkedList.h"

#include "ESP32WebServer.h"
#include "OBDIIContext.h"
#include "OBDIISettings.h"


/* 
 *  
 *  WiFi AP & Server
 *  
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiAP.h>
#include <esp_wifi.h>

// Forward Declarartions

// Globals
OBDIIContext gContext;
OBDIISettings gSettings;

BTClassicManager *gBTManager;
OBDIIWebServer *gServer;

bool gWiFiOn;

long gDiscoveryRequestStartTime;
long gDiscoveryStartTime;

void setup() {
  Serial.begin(115200);
  Serial.println();

  //delay(1000);

  // Init Bluetooth Classic
  gBTManager = new BTClassicManager();
  
  // Init WiFi
  WiFi.softAP(gSettings.ssid, gSettings.password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Start Web Server
  gServer = new OBDIIWebServer(gBTManager, &gContext, 80);
  gServer->begin();
  Serial.println("Server started");

  esp_wifi_stop();
  gWiFiOn = false;

  // Init Bluetooth Classic
  gBTManager = new BTClassicManager();

  gContext.bScanRequested = true;
  gDiscoveryRequestStartTime = 0;
  gDiscoveryStartTime = 0;
}

void loop() {
  static long task_start_ms = 0;
  static long alotted_discovery_time = 10000;

  bool discovering = (0 != task_start_ms);

  long now = millis();
  long time_delta = now - task_start_ms;

  // Scan for BT devices
  if (true == gContext.bScanRequested) {
    if (false == discovering) {
      // Stop WiFi
      if (true == gWiFiOn) {
        esp_wifi_stop();
        gWiFiOn = false;
        Serial.println("Stopping WiFi");
      }
      
      task_start_ms = now;
      gBTManager->StartDiscovery();
      gContext.bScanRequested = false;
      discovering = true;
      time_delta = now - task_start_ms;
    }
  }

  if (true == discovering) {
    // check discovery time
    if (time_delta > alotted_discovery_time) {
      task_start_ms = 0;
      gBTManager->StopDiscovery();
    }
  }

  // Turn on WiFi
  if (false == gWiFiOn && false == discovering) {
    esp_wifi_start();
    gWiFiOn = true;
    Serial.println("Starting WiFi");
  }

  // Handle Server Requests
  if (true == gWiFiOn) {
    gServer->handleClient();
  }
}
