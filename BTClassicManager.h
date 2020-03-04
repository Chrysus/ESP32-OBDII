#include <Arduino.h>

// Forward Declarations
class BTClassicDevice;
class BTClassicDeviceLinkedList;

// BTClassicManager Class

class BTClassicManager {
  public:

  // Constructor
  BTClassicManager();
  ~BTClassicManager();

  void StartDiscovery();
  void StopDiscovery();

  // HACK
  BTClassicDeviceLinkedList * GetDeviceList();


  private:
  char* deviceName;
  
  static BTClassicDeviceLinkedList *deviceList;

  // Bluetooth Classic (btc)
  void btc_init(char *device_name);
  void btc_start_discovery();
  void btc_stop_discovery();
  void btc_finish();
  
  static void btc_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
  
  static void btc_event_callback_discovery_result(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_discovery_state_changed(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_remote_service(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_remote_service_record(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_auth_complete(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_pin_code_request(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_pairing_confirmation_request(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_pairing_passkey_notification(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_pairing_passkey_request(esp_bt_gap_cb_param_t *param);
  static void btc_event_callback_read_rssi(esp_bt_gap_cb_param_t *param);

};
