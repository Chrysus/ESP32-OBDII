#include <WebServer.h>
#include "OBDIIContext.h"

// Forward Declarations
class BTClassicManager;

class OBDIIWebServer : public WebServer {
  public:
  OBDIIWebServer(BTClassicManager *btManager, OBDIIContext *context, int port = 80);
  ~OBDIIWebServer();

  private:
  BTClassicManager *BTManager;
  OBDIIContext *context;
  
  void HandleRoot();
  void HandleNotFound();
  void HandleScan();
  void HandleDevices();
  void HandlePair();
  void HandleUnpair();
  void HandleSettings();
};
