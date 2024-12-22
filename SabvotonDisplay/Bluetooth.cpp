#include "Bluetooth.h"

void Bluetooth::init(void) {
  // Set BLE device name
  BLE.setDeviceName("Sabvoton Display");

  // Set the local name that appears in BLE scans
  BLE.setLocalName("Sabvoton Display");

  // Initialize BLE
  if (!BLE.begin()) {
    ErrorHandler::raise_fatal_error("Failed to initialize BLE");
    return;
  }

  // Add characteristics to the service
  this->sabvotonDisplayService.addCharacteristic(this->batteryVoltageCharacteristic);
  this->sabvotonDisplayService.addCharacteristic(this->speedCharacteristic);

  // Add the eBike service
  BLE.setAdvertisedService(this->sabvotonDisplayService);
  BLE.addService(this->sabvotonDisplayService);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE initialized and advertising");
}

void Bluetooth::poll(void) {
  // Maintain BLE connection
  BLE.poll();
}

void Bluetooth::write_battery_voltage(float batteryVoltage) {
  this->batteryVoltageCharacteristic.writeValue(batteryVoltage);
}

void Bluetooth::write_speed(int speed) {
  this->speedCharacteristic.writeValue(speed);
}