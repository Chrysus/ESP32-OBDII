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

// State Machine
typedef enum {
  OBDII_INIT = 0,
  OBDII_WIFI_SERVER,
  OBDII_WIFI_UPLOAD,
  OBDII_BT_DISCOVERY,
  OBDII_BT_SERIAL,
} SystemState;

// Forward Declarartions
void UpdateStateMachine();
SystemState EvalSystemState_Init();
SystemState EvalSystemState_WifiServer();
SystemState EvalSystemState_WifiUpload();
SystemState EvalSystemState_BtDiscovery();
SystemState EvalSystemState_BtSerial();

void StartWiFi();
void StopWiFi();

// Globals
SystemState gSystemState;

OBDIIContext gContext;
OBDIISettings gSettings;

BTClassicManager *gBTManager;
OBDIIWebServer *gServer;

bool gBTOn;
bool gWiFiOn;

void setup() {
  Serial.begin(115200);
  Serial.println();

  //delay(1000);

  gSystemState = SystemState::OBDII_INIT;

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

  gWiFiOn = true;
  StopWiFi();

  // Init Bluetooth Classic
  gBTManager = new BTClassicManager();
  gContext.bScanRequested = false;
}

void loop() {
  UpdateStateMachine();
}

/*
 * State Machine
 */

void UpdateStateMachine() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  switch (cur_state) {
    case SystemState::OBDII_INIT:
    {
      next_state = EvalSystemState_Init();
      break;
    }
    case SystemState::OBDII_WIFI_SERVER:
    {
      next_state = EvalSystemState_WifiServer();
      break;
    }
    case SystemState::OBDII_WIFI_UPLOAD:
    {
      next_state = EvalSystemState_WifiUpload();
      break;
    }
    case SystemState::OBDII_BT_DISCOVERY:
    {
      next_state = EvalSystemState_BtDiscovery();
      break;
    }
    case SystemState::OBDII_BT_SERIAL:
    {
      next_state = EvalSystemState_BtSerial();
      break;
    }
    default:
    {
      next_state = SystemState::OBDII_INIT;
      Serial.println("ERROR: invalid system state selected!");
      break;
    }
  }

  if (next_state != gSystemState) {
    Serial.printf("STATE CHANGE: %i >> %i\n", gSystemState, next_state);
    gSystemState = next_state;
  }
}

/*
 * State Evaluators
 */

// ***** OBDII_INIT *****
SystemState EvalSystemState_Init() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  // After init, start discovery
  next_state = OBDII_BT_DISCOVERY;

  return next_state;
}

// ***** OBDII_WIFI_SERVER *****
SystemState EvalSystemState_WifiServer() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  // Handle Server Requests
  if (false == gWiFiOn) {
    StartWiFi();
  } else {
    gServer->handleClient();
  }

  // Discovery Request
  if (true == gContext.bScanRequested) {
    next_state = SystemState::OBDII_BT_DISCOVERY;
  }

  return next_state;
}

// ***** OBDII_WIFI_UPLOAD *****
SystemState EvalSystemState_WifiUpload() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  // TODO

  return next_state;
}

// ***** OBDII_BT_DISCOVERY *****
SystemState EvalSystemState_BtDiscovery() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  static long sDiscoveryStartTime = 0;

  // WiFi
  if (true == gWiFiOn) {
    StopWiFi();
  }

  // Discovery Request Reset
  if (true == gContext.bScanRequested) {
    gContext.bScanRequested = false;
  }

  if (0 == sDiscoveryStartTime) {
    gBTManager->StartDiscovery();
    sDiscoveryStartTime = millis();
  }

  // Timeout
  long now = millis();
  long timeDelta = now - sDiscoveryStartTime;
  
  if (timeDelta > gContext.discoveryDuration) {
    gBTManager->StopDiscovery();
    sDiscoveryStartTime = 0;
    next_state = SystemState::OBDII_WIFI_SERVER;
  }

  return next_state;
}

// ***** OBDII_BT_SERIAL *****
SystemState EvalSystemState_BtSerial() {
  SystemState cur_state = gSystemState;
  SystemState next_state = cur_state;

  // TODO

  return next_state;  
}

/*
 * Radio Management-ish
 */

void StartWiFi() {
  if (true == gBTOn) {
    Serial.println("ERROR: Cannot Start WiFi (BT is active)");
    return;
  }
  
  if (false == gWiFiOn && false == gBTOn) {
    esp_wifi_start();
    gWiFiOn = true;
    Serial.println("Starting WiFi");
  }  
}

void StopWiFi() {
  if (true == gWiFiOn) {
    esp_wifi_stop();
    gWiFiOn = false;
    Serial.println("Stopping WiFi");
  }
}
