#ifndef Mode_h
#define Mode_h

#include "Arduino.h"

class Mode {
  public:
    // Fields for a mode
    char name[16];
    bool throttleEnabled;
    bool pasEnabled;
    int dcCurrent;
    float speedLimit;

    // Constructor
    Mode(bool throttle = false, bool pas = false, int current = 0, float limit = 0.0)
        : throttleEnabled(throttle), pasEnabled(pas), dcCurrent(current), speedLimit(limit) {}

    // Serialize mode to a byte array for BLE or EEPROM
    void serialize(uint8_t *buffer) const;

    // Deserialize mode from a byte array
    void deserialize(const uint8_t *buffer);
};

#endif