#ifndef Bluetooth_h
#define Bluetooth_h

#include "Arduino.h"
#include <ArduinoBLE.h>
#include "Definitions.h"
#include "ErrorHandler.h"

class Bluetooth {
  public:
    //Functions
    void init(void);
    void poll(void);
    void write_battery_voltage(float batteryVoltage);
    void write_speed(int speed);
  private:
    // BLE service and characteristic definitions
    BLEService sabvotonDisplayService{"623304fc-a720-4d25-b50b-db235188ef6e"};
    BLEFloatCharacteristic batteryVoltageCharacteristic{"dc31e889-6d41-4a09-9b6a-2a027fb3ed4c", BLERead};
    BLEIntCharacteristic speedCharacteristic{"fc56d22a-1e95-43c9-8844-826285950c7c", BLERead};
};

#endif