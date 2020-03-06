/*
 * OBDIIContext.h
 */

#ifndef OBDIICONTEXT_H
#define OBDIICONTEXT_H

struct OBDIIContext {
  bool bScanRequested = false;

  long discoveryDuration = 1000 * 10;
};

#endif
