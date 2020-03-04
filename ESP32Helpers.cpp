#include "ESP32Helpers.h"

#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

void print_dev_prop(esp_bt_gap_dev_prop_t *prop) {
  esp_bt_gap_dev_prop_type_t type = prop->type;

  int prop_length = prop->len;
  char *buf = NULL;
  
  switch (type) {
    case ESP_BT_GAP_DEV_PROP_BDNAME:
    // Name
    buf = (char *)calloc(prop_length + 1, sizeof(char));
    memcpy(buf, prop->val, prop_length);
 
    Serial.printf("DEVICE NAME: %s\n", buf);
    free(buf);
    break;
    case ESP_BT_GAP_DEV_PROP_COD:
    Serial.printf("DEVICE COD: %i\n", (uint32_t *)prop->val);
    break;
    case ESP_BT_GAP_DEV_PROP_RSSI:
    Serial.printf("DEVICE RSSI: %i\n", (int8_t *)prop->val);
    break;
    case ESP_BT_GAP_DEV_PROP_EIR:
    // Extended Inquiry Response
    print_eir(prop);
    break;
    default:
    break;
  }
}

void print_eir(esp_bt_gap_dev_prop_t *prop) {
  // TODO
  uint8_t name_length = 0;
  char name_buffer[ESP_BT_GAP_MAX_BDNAME_LEN];
  
  uint8_t *eir = (uint8_t *)(prop->val);
  char *name_buffer_ptr = name_buffer;
  
  bool success = get_name_from_eir(eir, name_buffer_ptr, &name_length);
  if (success) {
    Serial.printf("EIR DEVICE NAME: %s\n", name_buffer);
  }

  /*
  int prop_length = prop->len;
  char *buf = NULL;
  
  buf = (char *)calloc(prop_length + 1, sizeof(char));
  memcpy(buf, prop->val, prop_length);
  Serial.printf("DEVICE EIR: %s\n", buf);
  
  free(buf);
  */
}

void print_mac_address(const uint8_t *mac_address) {
  for (int i = 0; i < 5; i++) Serial.printf("%02x:", mac_address[i]);
  Serial.printf("%02x", mac_address[5]);
}

bool get_name_from_eir(uint8_t *eir, char *bdname, uint8_t *bdname_len) {
  
    if (!eir || !bdname || !bdname_len) {
        return false;
    }
    
    *bdname = NULL;
    *bdname_len = NULL;

    uint8_t *rmt_bdname = NULL; 
    uint8_t rmt_bdname_len = 0;

    rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname) {
        rmt_bdname = esp_bt_gap_resolve_eir_data(eir, ESP_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }
    
    if (rmt_bdname) {
        rmt_bdname_len = (rmt_bdname_len > ESP_BT_GAP_MAX_BDNAME_LEN) ? ESP_BT_GAP_MAX_BDNAME_LEN : rmt_bdname_len;
        memcpy(bdname, rmt_bdname, rmt_bdname_len);
        bdname[rmt_bdname_len] = 0;
        *bdname_len = rmt_bdname_len;
        return true;
    }
    return false;
}
