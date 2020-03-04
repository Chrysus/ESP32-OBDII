#include <Arduino.h>

#include "esp_gap_bt_api.h"

// Helpers
void print_dev_prop(esp_bt_gap_dev_prop_t *prop);
void print_eir(esp_bt_gap_dev_prop_t *prop);
void print_mac_address(const uint8_t *mac_address);
bool get_name_from_eir(uint8_t *eir, char *bdname, uint8_t *bdname_len);
