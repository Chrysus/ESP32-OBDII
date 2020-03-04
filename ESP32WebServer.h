#include <WebServer.h>
#include "OBDIIContext.h"

// Forward Declarations
class BTClassicManager;

class OBDIIWebServer : public WebServer {
  public:
  OBDIIWebServer(BTClassicManager *btManager, int port = 80);
  ~OBDIIWebServer();

  // HACK
  void Init();
  void SetBTManager(BTClassicManager *btManager);

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
