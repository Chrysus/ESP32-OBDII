#include "OBDIIWebPageDevices.h"
#include "BTClassicDevice.h"
#include "BTClassicDeviceLinkedList.h"
#include "ESP32Helpers.h"


String* OBDIIWebPageDevicesString(BTClassicDeviceLinkedList *device_list) {
  String *message = new String();

  *message += "<h1>Found Devices:</h1>\n\n";

  BTClassicDeviceLinkedListNode *cur_node = NULL;
  
  if (device_list != NULL) {
    cur_node = device_list->GetHead();

    // DEBUG
    device_list->PrintList();
  } else {
    Serial.println("DEVICE LIST IS NULL!");
  }

  String *mac_address = NULL;
  while (cur_node != NULL) {
    if (cur_node->device != NULL) {
      if (cur_node->device->device_name != NULL) {
        *message += "<h3>";
        *message += cur_node->device->device_name;
        *message += "\n";
        *message += "</h3>";
      } else {
        
      }

      /*
      mac_address = mac_address_string(cur_node->device->mac_address);
      *message += "<H4>";
      *message += *mac_address;
      *message += "\n";
      *message += "</H4>";
      
      delete(mac_address);
      */
    }
    
    cur_node = cur_node->next;
  }


  return message;
}
