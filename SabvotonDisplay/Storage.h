#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <EEPROM.h>
#include "Definitions.h"

class Storage {
  public:
    static void clear_all();
    static void clear(int address, size_t length);
    static void writeBlock(size_t address, const uint8_t *data, size_t length);
    static void readBlock(size_t address, uint8_t *data, size_t length);

  private:
    size_t capacity;
};

#endif
