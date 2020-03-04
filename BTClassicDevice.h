#include <Arduino.h>

// BTClassicDevice Class

class BTClassicDevice {
  public:

  // Constructors
  BTClassicDevice();
  BTClassicDevice(BTClassicDevice *device);
  BTClassicDevice(void *param); //<< param should be a esp_bt_gap_cb_param_t*
  ~BTClassicDevice();

  void UpdateDevice(BTClassicDevice *device);

  uint8_t device_name_len;
  char *device_name;
  uint8_t mac_address[6];

  void SetDeviceName(char *device_name, uint8_t len);
  void SetDeviceAddress(uint8_t *device_address);

  void printInfo();
};
