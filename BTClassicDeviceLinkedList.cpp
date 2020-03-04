#include "BTClassicDeviceLinkedList.h"
#include "BTClassicDevice.h"


/*
 * 
 * BTClassicDeviceLinkedListNode
 * 
 */

BTClassicDeviceLinkedListNode::BTClassicDeviceLinkedListNode() {
  this->device = NULL;
  this->next = NULL;
}

BTClassicDeviceLinkedListNode::~BTClassicDeviceLinkedListNode() {
  if (this->device != NULL) {
    delete(this->device);
    this->device = NULL;
  }

  if (this->next != NULL) {
    delete(this->next);
    this->next = NULL;
  }
}


/*
 * 
 * BTClassicDeviceLinkedList
 * 
 */

BTClassicDeviceLinkedList::BTClassicDeviceLinkedList() {
  this->head = NULL;
  this->tail = NULL;
}

BTClassicDeviceLinkedList::~BTClassicDeviceLinkedList() {
  BTClassicDeviceLinkedListNode *curNode = this->head;
  while (curNode != NULL) {
    this->head = curNode->next;
    curNode->next = NULL;
    delete(curNode);
    curNode = this->head;
  }
}

void BTClassicDeviceLinkedList::AddDevice(BTClassicDevice *device) {
  BTClassicDevice *_device = new BTClassicDevice(device);
  BTClassicDeviceLinkedListNode *node = new BTClassicDeviceLinkedListNode();

  node->device = _device;

  if (this->tail == NULL) {
    this->head = node;
    this->tail = node;
  } else {
    this->tail->next = node;
    this->tail = node;
  }
}

void BTClassicDeviceLinkedList::RemoveDevice(BTClassicDevice *device) {
  // TODO
}

BTClassicDevice * BTClassicDeviceLinkedList::FindDeviceByName(const char *device_name) {
  // TODO
  return NULL;
}
BTClassicDevice * BTClassicDeviceLinkedList::FindDeviceByAddress(const uint8_t *device_address) {  
  BTClassicDeviceLinkedListNode *curNode = this->head;
  BTClassicDevice *curDevice = NULL;
  BTClassicDevice *foundDevice = NULL;
  
  while (curNode != NULL) {
    curDevice = curNode->device;
    if (memcmp(curDevice->mac_address, device_address, 6) == 0) {
      foundDevice = curDevice;
      break;
    }
    curNode = curNode->next;
  }
  
  return foundDevice;
}

BTClassicDeviceLinkedListNode * BTClassicDeviceLinkedList::GetHead() {
  return this->head;
}

void BTClassicDeviceLinkedList::PrintList() {
  BTClassicDeviceLinkedListNode *curNode = this->head;
  BTClassicDevice *curDevice = NULL;
  
  while (curNode != NULL) {
    curDevice = curNode->device;
    if (curDevice != NULL) {
      curDevice->printInfo();
    }
    curNode = curNode->next;
  }
}
