#include "ModeManager.h"

ModeManager::ModeManager() {
  // loadModes();
}

void ModeManager::addMode(const Mode &mode) {
  modes.push_back(mode);
}

void ModeManager::removeMode(size_t index) {
  if (index < modes.size()) {
    modes.erase(modes.begin() + index);
  }
}

Mode *ModeManager::getMode(size_t index) const {
  return (index < modes.size() && index >= 0) ? const_cast<Mode*>(&modes[index]) : nullptr;
}

void ModeManager::saveCurrentMode() {
  Serial.println("Saving current mode to EEPROM");
  Storage::clear(current_mode_index * MODE_PARAMS_SIZE_BYTES, MODE_PARAMS_SIZE_BYTES);
  uint8_t buffer[MODE_PARAMS_SIZE_BYTES];
  modes[current_mode_index].serialize(buffer);
  Storage::writeBlock(current_mode_index * MODE_PARAMS_SIZE_BYTES, buffer, MODE_PARAMS_SIZE_BYTES);
  Serial.println("Successfully saved current mode to EEPROM");
}

void ModeManager::saveModes() {
  Serial.println("Saving modes to EEPROM");
  int saveCount = 0;
  Storage::clear_all();
  for (size_t i = 0; i < modes.size(); ++i) {
    uint8_t buffer[MODE_PARAMS_SIZE_BYTES];
    modes[i].serialize(buffer);
    Storage::writeBlock(i * MODE_PARAMS_SIZE_BYTES, buffer, MODE_PARAMS_SIZE_BYTES);
    saveCount++;
  }
  Serial.print("Successfully saved: ");
  Serial.print(saveCount);
  Serial.println(" mode/s to EEPROM");
}

void ModeManager::loadModes() {
  Serial.println("Loading modes from EEPROM");
  int loadCount = 0;
  modes.clear();
  uint8_t buffer[MODE_PARAMS_SIZE_BYTES];
  for (size_t address = 0; address < EEPROM_USABLE_CAPACITY_BYTES; address += MODE_PARAMS_SIZE_BYTES) {
    Storage::readBlock(address, buffer, MODE_PARAMS_SIZE_BYTES);
    if (buffer[0] == 0xFF) break;  // End of stored modes
    Mode mode;
    mode.deserialize(buffer);
    modes.push_back(mode);
    Serial.println("Found stored mode: ");
    char textBuf[255];
    sprintf(textBuf, "Name: %s, Throttle Enabled: %d, PAS Enabled: %d, DC Current: %d, Speed Limit: %.2f", mode.name, mode.throttleEnabled, mode.pasEnabled, mode.dcCurrent, mode.speedLimit);
    Serial.println(textBuf);
    loadCount++;
  }
  Serial.print("Successfully loaded: ");
  Serial.print(loadCount);
  Serial.println(" modes from EEPROM");
}

size_t ModeManager::getModeCount() const {
  return modes.size();
}

void ModeManager::selectMode(int index) {
  if((index + 1) > this->getModeCount()) {
    return;
  }
  current_mode_index = index;
}
