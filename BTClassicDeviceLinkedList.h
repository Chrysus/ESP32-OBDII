#include <Arduino.h>

// Forward Declarations
class BTClassicDevice;

class BTClassicDeviceLinkedListNode {
  public:
  BTClassicDeviceLinkedListNode();
  ~BTClassicDeviceLinkedListNode();

  BTClassicDevice *device;
  BTClassicDeviceLinkedListNode *next;
};

class BTClassicDeviceLinkedList {
  public:
  BTClassicDeviceLinkedList();
  ~BTClassicDeviceLinkedList();

  void AddDevice(BTClassicDevice *device);
  void RemoveDevice(BTClassicDevice *device);

  BTClassicDevice *FindDeviceByName(const char *device_name);
  BTClassicDevice *FindDeviceByAddress(const uint8_t *device_address);

  // HACK
  BTClassicDeviceLinkedListNode * GetHead();
  void PrintList();

  private:
  BTClassicDeviceLinkedListNode *head;
  BTClassicDeviceLinkedListNode *tail;
};
