#include "Storage.h"

void Storage::clear_all() {
  Storage::clear(0, EEPROM_USABLE_CAPACITY_BYTES);
  // EEPROM.commit();
}

void Storage::clear(int address, size_t length) {
  Serial.print("Clearing EEPROM: ");
  Serial.print(length);
  Serial.print(" bytes from address: ");
  Serial.println(address);
  for (size_t i = address; i < (address + length); ++i) {
      EEPROM.write(i, 0xFF);  // Mark as empty
  }
  Serial.println("Finished clearing EEPROM");
}

void Storage::writeBlock(size_t address, const uint8_t *data, size_t length) {
  Serial.print("Writing to EEPROM at address: ");
  Serial.print(address);
  Serial.print(", length: ");
  Serial.print(length);
  Serial.println(" bytes");
  for (size_t i = 0; i < length; ++i) {
      EEPROM.write(address + i, data[i]);
  }
  Serial.println("Finished writing to EEPROM");
  // EEPROM.commit();
}

void Storage::readBlock(size_t address, uint8_t *data, size_t length) {
  Serial.print("Reading from EEPROM at address: ");
  Serial.print(address);
  Serial.print(", length: ");
  Serial.print(length);
  Serial.println(" bytes");
  for (size_t i = 0; i < length; ++i) {
      data[i] = EEPROM.read(address + i);
  }
  Serial.println("Finished reading from EEPROM");
}
