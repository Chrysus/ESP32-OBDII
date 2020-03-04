#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

#include "BTClassicDevice.h"
#include "BTClassicManager.h"
#include "BTClassicDeviceLinkedList.h"

BTClassicDeviceLinkedList * BTClassicManager::deviceList = NULL;

BTClassicManager::BTClassicManager() {
  const char* device_name = "ESP32_BT_CLASSIC";
  this->deviceName = (char *)calloc(strlen(device_name), sizeof(char));
  strcpy(this->deviceName, device_name);

  if (deviceList == NULL) {
    deviceList = new BTClassicDeviceLinkedList();
  }

  this->btc_init(this->deviceName);
}

BTClassicManager::~BTClassicManager() {
  
}

void BTClassicManager::StartDiscovery() {
  this->btc_start_discovery();
}

void BTClassicManager::StopDiscovery() {
  this->btc_stop_discovery();
}

BTClassicDeviceLinkedList * BTClassicManager::GetDeviceList() {
  return BTClassicManager::deviceList;
}


void BTClassicManager::btc_init(char *device_name) {
  esp_err_t esp_result;

  // Start Bluetooth
  bool started = btStart();
  if (!started) {
    Serial.printf("ERROR: Unable to start bluetooth\n");
    return;
  }

  // Init
  esp_result = esp_bluedroid_init();
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to init bluetooth\n", esp_result);
    return;
  }

  // Enable
  esp_result = esp_bluedroid_enable();
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to enable bluetooth\n", esp_result);
    return;
  }

  // Device Name
  esp_result = esp_bt_dev_set_device_name(device_name);
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to set device name\n", esp_result);
    return;
  }

  esp_result = esp_bt_gap_register_callback(this->btc_callback);
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to register callback\n", esp_result);
    return;
  }
}

void BTClassicManager::btc_finish() {
  esp_err_t esp_result;

  // Disable
  esp_result = esp_bluedroid_disable();
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to disable bluetooth\n", esp_result);
    return;
  }

  // De-init
  esp_result = esp_bluedroid_deinit();
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to de-init bluetooth\n", esp_result);
    return;
  }

  // Stop Bluetooth
  bool stopped = btStop();
  if (!stopped) {
    Serial.printf("ERROR: Unable to stop bluetooth\n");
  }
}

void BTClassicManager::btc_start_discovery() {
  esp_err_t esp_result;

  Serial.printf("Request: Start Discovery\n");

  // Check that Bluetooth is Initialized and Enabled
  esp_bluedroid_status_t btc_status = esp_bluedroid_get_status();
  if (ESP_BLUEDROID_STATUS_ENABLED != btc_status) {
    Serial.printf("ERROR (%i): bluetooth initialized and enabled\n", btc_status);
    return;
  }

  // SCAN MODES
  //  * ESP_BT_SCAN_MODE_NONE                       * Does not scan
  //  * ESP_BT_SCAN_MODE_CONNECTABLE                * Scans for known connectable devices
  //  * ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE   * Scans for discoverable devices
  
  esp_result = esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to set scan mode\n", esp_result);
    return;
  }


  // INQUERY MODES
  //  * ESP_BT_INQ_MODE_GENERAL_INQUIRY
  //  * ESP_BT_INQ_MODE_LIMITED_INQUIRY
  
  esp_result = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 0x30, 10); // (Mode, inq_len, num_rsps)
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to start discovery\n", esp_result);
    return;
  }
}

void BTClassicManager::btc_stop_discovery() {
  esp_err_t esp_result;

  Serial.printf("Request: Stop Discovery\n");
  
  esp_result = esp_bt_gap_cancel_discovery();
  if (ESP_OK != esp_result) {
    Serial.printf("ERROR (%i): Unable to stop discovery\n", esp_result);
    return;
  }
}

void BTClassicManager::btc_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {

  switch (event) {
    case ESP_BT_GAP_DISC_RES_EVT:
    /*!< device discovery result event */
    //Serial.printf("EVENT: Discovery Result\n");
    btc_event_callback_discovery_result(param);
    break;
    case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:
    /*!< discovery state changed event */
    //Serial.printf("EVENT: Discovery State Changed\n");
    btc_event_callback_discovery_state_changed(param);
    break;
    case ESP_BT_GAP_RMT_SRVCS_EVT:
    /*!< get remote services event */
    //Serial.printf("EVENT: Remote Service\n");
    btc_event_callback_remote_service(param);
    break;
    case ESP_BT_GAP_RMT_SRVC_REC_EVT:
    /*!< get remote service record event */
    //Serial.printf("EVENT: Service Record\n");
    btc_event_callback_remote_service_record(param);
    break;
    case ESP_BT_GAP_AUTH_CMPL_EVT:
    /*!< AUTH complete event */
    //Serial.printf("EVENT: AUTH Complete\n");
    btc_event_callback_auth_complete(param);
    break;
    case ESP_BT_GAP_PIN_REQ_EVT:
    /*!< Legacy Pairing Pin code request */
    //Serial.printf("EVENT: PIN Code Request\n");
    btc_event_callback_pin_code_request(param);
    break;
    case ESP_BT_GAP_CFM_REQ_EVT:
    /*!< Simple Pairing User Confirmation request. */
    //Serial.printf("EVENT: Pairing Confirmation Request\n");
    btc_event_callback_pairing_confirmation_request(param);
    break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
    /*!< Simple Pairing Passkey Notification */
    //Serial.printf("EVENT: Passkey Notification\n");
    btc_event_callback_pairing_passkey_notification(param);
    break;
    case ESP_BT_GAP_KEY_REQ_EVT:
    /*!< Simple Pairing Passkey request */
    //Serial.printf("EVENT: Passkey Request\n");
    btc_event_callback_pairing_passkey_request(param);
    break;
    case ESP_BT_GAP_READ_RSSI_DELTA_EVT:
    /*!< read rssi event */
    //Serial.printf("EVENT: Read RSSI Delta\n");
    btc_event_callback_read_rssi(param);
    break;
    /*
    case ESP_BT_GAP_CONFIG_EIR_DATA_EVT:
    break;
    */
    /*!< config EIR data event */
    /*
    case ESP_BT_GAP_SET_AFH_CHANNELS_EVT:
    break;
    */
    /*!< set AFH channels event */
    /*
    case ESP_BT_GAP_READ_REMOTE_NAME_EVT:
    break;
    */
    /*!< read Remote Name event */
    default:
    Serial.printf("EVENT: Unknown\n");
    break;
  }
}


//-------------------------------------------------------------
void BTClassicManager::btc_event_callback_discovery_result(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Discovery Result");

  BTClassicDevice *found_device = new BTClassicDevice(param);

  Serial.printf("Found Device!\n");
  found_device->printInfo();

  if (deviceList == NULL) {
    Serial.printf("NULL DEVICE LIST!\n");
    delete(found_device);
    return;
  }
  
  BTClassicDevice *known_device = deviceList->FindDeviceByAddress(found_device->mac_address);
  if (known_device == NULL) {
    deviceList->AddDevice(found_device);
  } else {
    if (known_device->device_name == NULL && found_device->device_name != NULL) {
      known_device->UpdateDevice(found_device);
    }
  }
}

void BTClassicManager::btc_event_callback_discovery_state_changed(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Discovery State Changed");

  // STATES
  //  * ESP_BT_GAP_DISCOVERY_STOPPED
  //  * ESP_BT_GAP_DISCOVERY_STARTED

  esp_bt_gap_discovery_state_t state = param->disc_st_chg.state;

  switch (state) {
    case ESP_BT_GAP_DISCOVERY_STOPPED:
    Serial.println("    * DISCOVERY STOPPED");
    break;
    case ESP_BT_GAP_DISCOVERY_STARTED:
    Serial.println("    * DISCOVERY STARTED");
    break;
    default:
    Serial.println("    * DISCOVERY UNKNOWN STATE");
    break;
  }
}

void BTClassicManager::btc_event_callback_remote_service(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Remote Service Event");
}

void BTClassicManager::btc_event_callback_remote_service_record(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Remote Service Record");
}

void BTClassicManager::btc_event_callback_auth_complete(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Auth Complete");
}

void BTClassicManager::btc_event_callback_pin_code_request(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: PIN Code Requested");
}

void BTClassicManager::btc_event_callback_pairing_confirmation_request(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Pairing Confirmation Requested");
}

void BTClassicManager::btc_event_callback_pairing_passkey_notification(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Passkey Notification");
}

void BTClassicManager::btc_event_callback_pairing_passkey_request(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Passkey Requested");
}

void BTClassicManager::btc_event_callback_read_rssi(esp_bt_gap_cb_param_t *param) {
  Serial.println("CALLBACK: Read RSSI");
}
