#include "BTClassicDevice.h"
#include "ESP32Helpers.h"

#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

BTClassicDevice::BTClassicDevice() {
  this->device_name_len = 0;
  this->device_name = NULL;
}

BTClassicDevice::BTClassicDevice(BTClassicDevice *device) {

  this->device_name_len = 0;
  this->device_name = NULL;
  
  char *buf = NULL;
  int name_length = 0;

  if (device == NULL) {
    return;
  }

  if (device->device_name != NULL) {
    name_length = device->device_name_len;
    buf = (char *)calloc(name_length + 1, sizeof(char));
    memcpy(buf, device->device_name, name_length);
  }
  
  this->device_name_len = name_length;
  this->device_name = buf;
  
  memcpy(this->mac_address, device->mac_address, 6);
}

BTClassicDevice::BTClassicDevice(void *param) {

  this->device_name_len = 0;
  this->device_name = NULL;

  esp_bt_gap_cb_param_t *gap_cb_param = (esp_bt_gap_cb_param_t *)param;

  // Address
  uint8_t *MAC_Address = this->mac_address;
  
  esp_bd_addr_t *device_address = &(gap_cb_param->disc_res.bda);
  memcpy(MAC_Address, (const uint8_t *)device_address, 6);

  // Properties
  esp_bt_gap_dev_prop_t *prop = NULL;
  esp_bt_gap_dev_prop_type_t prop_type;
  int prop_length;

  uint8_t buf_size = 0;
  char *buf = NULL;
  
  int property_count = gap_cb_param->disc_res.num_prop;
  esp_bt_gap_dev_prop_t *props = gap_cb_param->disc_res.prop;
  
  for (int i = 0; i < property_count; ++i) {
    prop = (props + i);
    prop_type = prop->type;
  
    switch (prop_type) {
      case ESP_BT_GAP_DEV_PROP_BDNAME:
      // Name
      buf = (char *)calloc(prop_length + 1, sizeof(char));
      memcpy(buf, prop->val, prop_length);
      this->device_name = buf;
      this->device_name_len = prop_length;
      buf = NULL;
      break;
      case ESP_BT_GAP_DEV_PROP_COD:
      // TODO
      break;
      case ESP_BT_GAP_DEV_PROP_RSSI:
      // TODO
      break;
      case ESP_BT_GAP_DEV_PROP_EIR:
      // Extended Inquiry Response
      // TODO - Get All Data Fields (for now, just getting device name)
      buf_size = (ESP_BT_GAP_MAX_BDNAME_LEN < prop_length) ? (prop_length + 1) : ESP_BT_GAP_MAX_BDNAME_LEN;
      buf = (char *)calloc(buf_size, sizeof(char));
      get_name_from_eir((uint8_t *)(prop->val), buf, &buf_size);
      this->SetDeviceName(buf, buf_size);
      free(buf);
      buf = NULL;
      break;
      default:
      break;
    }
  }
}

BTClassicDevice::~BTClassicDevice() {
  free(device_name);
}

void BTClassicDevice::UpdateDevice(BTClassicDevice *device) {
  // Sanity Check

  if (memcmp(this->mac_address, device->mac_address, 6) != 0) {
    // NOT THE SAME DEVICE!
    // TODO - handle error
    return;
  }

  if (NULL != device->device_name) {
    this->SetDeviceName(device->device_name, device->device_name_len);
  }
}

void BTClassicDevice::SetDeviceName(char *device_name, uint8_t len) {
  if (this->device_name != NULL) {
    free(this->device_name);
  }

  char *buf = (char *)calloc((len + 1), sizeof(char));
  memcpy(buf, device_name, len);
  this->device_name = buf;
  this->device_name_len = len;
}

void BTClassicDevice::SetDeviceAddress(uint8_t *device_address) {
  memcpy(this->mac_address, device_address, 6);
}

void BTClassicDevice::printInfo() {
  Serial.printf("DEVICE INFO:\n");
  if (NULL != this->device_name) {
    Serial.printf("    Name: %s\n", this->device_name);
  }

  Serial.printf("    Addr: ");
  print_mac_address(this->mac_address);
  Serial.printf("\n");
  //Serial.printf("    Addr: %s\n", this->address_string);
}
