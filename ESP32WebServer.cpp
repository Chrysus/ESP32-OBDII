#include "ESP32WebServer.h"

#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

#include "BTClassicDevice.h"
#include "BTClassicManager.h"
#include "BTClassicDeviceLinkedList.h"

#include "OBDIIWebPageScan.h"
#include "OBDIIWebPageDevices.h"

/* 
 *  WEB SERVER
 *  
 */

OBDIIWebServer::OBDIIWebServer(BTClassicManager *btManager, OBDIIContext *context, int port) 
: WebServer(port)
{
  this->BTManager = btManager;
  this->context = context;
  
  this->on("/", std::bind(&OBDIIWebServer::HandleRoot, this));
  this->on("/scan", std::bind(&OBDIIWebServer::HandleScan, this));
  this->on("/devices", std::bind(&OBDIIWebServer::HandleDevices, this));
  this->on("/pair", std::bind(&OBDIIWebServer::HandlePair, this));
  this->on("/unpair", std::bind(&OBDIIWebServer::HandleUnpair, this));
  this->on("/settings", std::bind(&OBDIIWebServer::HandleSettings, this));

  this->onNotFound(std::bind(&OBDIIWebServer::HandleNotFound, this));
}

OBDIIWebServer::~OBDIIWebServer() {
  
}

void OBDIIWebServer::HandleRoot() {
  BTClassicDeviceLinkedList *device_list = NULL;
  BTClassicDeviceLinkedListNode *cur_node = NULL;

  if (this->BTManager != NULL) {
    device_list = this->BTManager->GetDeviceList();
  }

  String message = "Found Devices:\n\n";

  if (device_list != NULL) {
    cur_node = device_list->GetHead();

    device_list->PrintList();
  } else {
    Serial.println("DEVICE LIST IS NULL!");
  }

  // HACK
  char *first_device_name = NULL;
  
  while (cur_node != NULL) {
    // TODO
    if (first_device_name == NULL) {
      if (cur_node->device != NULL) {
        if (cur_node->device->device_name != NULL) {
          first_device_name = cur_node->device->device_name;
        }
      }
    }

    if (cur_node->device->device_name != NULL) {
      message += cur_node->device->device_name;
      message += "\n";
    }
    
    cur_node = cur_node->next;
  }

  if (NULL != first_device_name) {
    this->send(200, "text/plain", message);
    return;
  }
  
  this->send(200, "text/plain", "hello from OBD2 Bridge!");
}

void OBDIIWebServer::HandleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += this->uri();
  message += "\nMethod: ";
  message += (this->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += this->args();
  message += "\n";
  for (uint8_t i = 0; i < this->args(); i++) {
    message += " " + this->argName(i) + ": " + this->arg(i) + "\n";
  }
  this->send(404, "text/plain", message);
}

void OBDIIWebServer::HandleScan() {
  this->context->bScanRequested = true;

  String *message = OBDIIWebPageScanString();
  this->send(200, "text/plain", *message);
  
  delete(message);
}

void OBDIIWebServer::HandleDevices() {
  BTClassicDeviceLinkedList *device_list = NULL;

  if (this->BTManager != NULL) {
    device_list = this->BTManager->GetDeviceList();
  }

  String *message = OBDIIWebPageDevicesString(device_list);
  this->send(200, "text/html", *message);

  delete(message);
}

void OBDIIWebServer::HandlePair() {
  this->send(200, "text/plain", "PAIR PAGE PLACEHODER!");
}

void OBDIIWebServer::HandleUnpair() {
  this->send(200, "text/plain", "UNPAIR PAGE PLACEHODER!");
}

void OBDIIWebServer::HandleSettings() {
  this->send(200, "text/plain", "SETTINGS PAGE PLACEHODER!");
}
