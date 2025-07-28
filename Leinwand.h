#ifndef _LEINWAND_H_
#define _LEINWAND_H_

#include <SinricProDevice.h>
#include <Capabilities/RangeController.h>

class Leinwand 
: public SinricProDevice
, public RangeController<Leinwand> {
  friend class RangeController<Leinwand>;
public:
  Leinwand(const String &deviceId) : SinricProDevice(deviceId, "Leinwand") {};
};

#endif
