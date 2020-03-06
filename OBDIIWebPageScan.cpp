#include "OBDIIWebPageScan.h"


String* OBDIIWebPageScanString() {
  String *message = new String();

  *message += "SCAN REQUESTED!";

  return message;
}
