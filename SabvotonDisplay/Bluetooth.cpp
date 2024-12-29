#include "Bluetooth.h"

Bluetooth::Bluetooth(ModeManager &modeManager) : modeManager(modeManager) {}

void Bluetooth::init(void) {
  // Append read characteristics
  characteristics[0] = {&battery_voltage_read_characteristic, false};
  characteristics[1] = {&speed_read_characteristic, false};
  characteristics[2] = {&mode_total_count_read_characteristic, false};
  characteristics[3] = {&mode_current_index_read_characteristic, false};
  characteristics[4] = {&mode_current_name_read_characteristic, false};
  characteristics[5] = {&mode_current_throttle_enabled_read_characteristic, false};
  characteristics[6] = {&mode_current_pas_enabled_read_characteristic, false};
  characteristics[7] = {&mode_current_dc_current_read_characteristic, false};
  characteristics[8] = {&mode_current_speed_limit_read_characteristic, false};

  // Append write characteristics
  characteristics[9] = {&mode_current_index_write_characteristic, true};
  characteristics[10] = {&mode_current_name_write_characteristic, true};
  characteristics[11] = {&mode_current_throttle_enabled_write_characteristic, true};
  characteristics[12] = {&mode_current_pas_enabled_write_characteristic, true};
  characteristics[13] = {&mode_current_dc_current_write_characteristic, true};
  characteristics[14] = {&mode_current_speed_limit_write_characteristic, true};

  // Set BLE device name
  BLE.setDeviceName("Sabvoton Display");

  // Set the local name that appears in BLE scans
  BLE.setLocalName("Sabvoton Display");

  // Initialize BLE
  if (!BLE.begin()) {
    ErrorHandler::raise_fatal_error("Failed to initialize BLE");
    return;
  }
  
  // Loop through all characteristics and add them to the SabvotonDisplay service
  for (const auto& charInfo : characteristics) {
    // Example: Add characteristics to the BLE service
    this->sabvotonDisplayService.addCharacteristic(*charInfo.characteristic);
}

  // Add the SabvotonDisplay service (with all attached characteristics)
  BLE.setAdvertisedService(this->sabvotonDisplayService);
  BLE.addService(this->sabvotonDisplayService);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE initialized and advertising");
}

void Bluetooth::poll(void) {
  // Maintain BLE connection (supposedly)
  BLE.poll();
}

void Bluetooth::write_battery_voltage(float batteryVoltage) {
  // Write to the battery voltage characteristic
  this->battery_voltage_read_characteristic.writeValue(batteryVoltage);
}

void Bluetooth::write_speed(float speed) {
  // Write to the speed characteristic
  this->speed_read_characteristic.writeValue(speed);
}

void Bluetooth::write_current_mode() {
  this->mode_total_count_read_characteristic.writeValue(modeManager.getModeCount());

  Mode *currentMode = modeManager.getMode(modeManager.current_mode_index);
  if(currentMode == nullptr) {
    modeManager.current_mode_index = -1;
    this->mode_current_index_read_characteristic.writeValue(-1);
    return;
  }
  
  this->mode_current_index_read_characteristic.writeValue(modeManager.current_mode_index);
  this->mode_current_name_read_characteristic.writeValue(currentMode->name);
  if(currentMode->throttleEnabled) {
    this->mode_current_throttle_enabled_read_characteristic.writeValue(1);
  } else {
    this->mode_current_throttle_enabled_read_characteristic.writeValue(0);
  }
  if(currentMode->pasEnabled) {
    this->mode_current_pas_enabled_read_characteristic.writeValue(1);
  } else {
    this->mode_current_pas_enabled_read_characteristic.writeValue(0);
  }
  this->mode_current_dc_current_read_characteristic.writeValue(currentMode->dcCurrent);
  this->mode_current_speed_limit_read_characteristic.writeValue(currentMode->speedLimit);
}

int Bluetooth::convert_characteristic_value_to_int32(const uint8_t* value, int length) {
  // Takes a read value from a characteristic and returns an integer
  int valueInt = 0;
  if(length == sizeof(int)) {
    memcpy(&valueInt, value, sizeof(int));
    return valueInt;
  }
  return 0;
}

float Bluetooth::convert_characteristic_value_to_float32(const uint8_t* value, int length) {
  // Takes a read value from a characteristic and returns a float
  float valueFloat = 0.0;
  if(length == sizeof(float)) {
    memcpy(&valueFloat, value, sizeof(float));
    return valueFloat;
  }
  return 0;
}

void Bluetooth::handle_write(BLECharacteristic *characteristic) {
  const char *characteristicUUID = characteristic->uuid();
  const uint8_t* value = characteristic->value();
  int length = characteristic->valueLength();

  bool modesNeedSave = false;
  Mode *currentMode = modeManager.getMode(modeManager.current_mode_index);
  if(currentMode == nullptr) {
    modeManager.current_mode_index = -1;
    return;
  }

  Serial.print("Update received from app for UUID: ");
  Serial.println(characteristicUUID);
  
  if(strcmp(characteristicUUID, mode_current_index_write_characteristic.uuid()) == 0) {
    // Current Mode index updated
    size_t currentModeCount = modeManager.getModeCount();
    int characteristicValue = this->convert_characteristic_value_to_int32(value, length);
    if((characteristicValue + 1) > currentModeCount) {
      // App has selected a mode that does not currently exist - assume it's creating a new mode
      Mode newMode;
      char newModeName[sizeof(newMode.name)];
      memset(newModeName, '\0', sizeof(newModeName));
      sprintf(newModeName, "NEW MODE");
      memcpy(newMode.name, newModeName, sizeof(newModeName));
      modeManager.addMode(newMode);
      modeManager.selectMode(currentModeCount);
      modesNeedSave = true;
    } else {
      modeManager.selectMode(characteristicValue);
    }
  } else if(strcmp(characteristicUUID, mode_current_name_write_characteristic.uuid()) == 0) {
    // Name updated
    memset(currentMode->name, '\0', sizeof(currentMode->name));
    if(length <= sizeof(currentMode->name)) {
      memcpy(currentMode->name, value, length);
    }
    modesNeedSave = true;
  } else if(strcmp(characteristicUUID, mode_current_throttle_enabled_write_characteristic.uuid()) == 0) {
    // Throttle Enabled updated
    int characteristicValue = this->convert_characteristic_value_to_int32(value, length);
    if(characteristicValue == 1) {
      currentMode->throttleEnabled = true;
    } else {
      currentMode->throttleEnabled = false;
    }
    modesNeedSave = true;
  } else if(strcmp(characteristicUUID, mode_current_pas_enabled_write_characteristic.uuid()) == 0) {
    // PAS Enabled updated
    int characteristicValue = this->convert_characteristic_value_to_int32(value, length);
    if(characteristicValue == 1) {
      currentMode->pasEnabled = true;
    } else {
      currentMode->pasEnabled = false;
    }
    modesNeedSave = true;
  } else if(strcmp(characteristicUUID, mode_current_dc_current_write_characteristic.uuid()) == 0) {
    // DC Current updated
    int characteristicValue = this->convert_characteristic_value_to_int32(value, length);
    currentMode->dcCurrent = characteristicValue;
    modesNeedSave = true;
  } else if(strcmp(characteristicUUID, mode_current_speed_limit_write_characteristic.uuid()) == 0) {
    // Speed Limit updated
    float characteristicValue = this->convert_characteristic_value_to_float32(value, length);
    currentMode->speedLimit = characteristicValue;
    modesNeedSave = true;
  }

  // modeManager.current_mode_updated = true;
  if(modesNeedSave) {
    modeManager.saveCurrentMode();
  }
}




