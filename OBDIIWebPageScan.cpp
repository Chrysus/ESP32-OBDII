#include "OBDIIWebPageScan.h"


String* OBDIIWebPageScanString() {
  String *message = new String();

  //*message += "SCAN REQUESTED!";
  *message += "<form action='/scan' method='POST'>";
  *message += "<input type='text' name='scan_time' size=16 maxlength=16'></input>";
  *message += "<input type='submit' value='Scan'></input>";
  *message += "</form>";

  return message;
}
