#ifndef Bluetooth_h
#define Bluetooth_h

#include "Arduino.h"
#include <ArduinoBLE.h>
#include "Mode.h"
#include "ModeManager.h"
#include "Definitions.h"
#include "ErrorHandler.h"

class Bluetooth {
  public:
    Bluetooth(ModeManager &modeManager);

    // General Functions
    void init(void);
    void poll(void);

    // Characteristic functions
    void write_battery_voltage(float batteryVoltage);
    void write_speed(float speed);
    void write_current_mode();

    void handle_write(BLECharacteristic *characteristic);

    struct BLECharacteristicInfo {
      BLECharacteristic* characteristic;
      bool isWrite;
    };

    BLECharacteristicInfo characteristics[15];
    
  private:
    // General service and characteristic definitions
    BLEService sabvotonDisplayService{"623304fc-a720-4d25-b50b-db235188ef6e"};
    BLEFloatCharacteristic battery_voltage_read_characteristic{"dc31e889-6d41-4a09-9b6a-2a027fb3ed4c", BLERead | BLENotify};
    BLEFloatCharacteristic speed_read_characteristic{"fc56d22a-1e95-43c9-8844-826285950c7c", BLERead | BLENotify};
    
    // Mode characteristics (read)
    BLEIntCharacteristic mode_total_count_read_characteristic{"de985840-90d8-4058-9c98-b0f83d8d32fa", BLERead | BLENotify};
    BLEIntCharacteristic mode_current_index_read_characteristic{"3f2b6f19-a5cd-44f1-8107-8d3e00635920", BLERead | BLENotify};
    BLECharacteristic mode_current_name_read_characteristic{"ecad590f-a8ff-4558-9df7-7b6034791887", BLERead | BLENotify, 16};
    BLEIntCharacteristic mode_current_throttle_enabled_read_characteristic{"f7d76def-31ae-48f8-a975-d374fac68178", BLERead | BLENotify};
    BLEIntCharacteristic mode_current_pas_enabled_read_characteristic{"96f55a19-8341-4895-91d3-81f25630eb91", BLERead | BLENotify};
    BLEIntCharacteristic mode_current_dc_current_read_characteristic{"7fd68eae-d802-4502-8c4e-e0d9769b8cbd", BLERead | BLENotify};
    BLEFloatCharacteristic mode_current_speed_limit_read_characteristic{"6c05fcf6-d854-4ba0-b71d-3b8afcf79c99", BLERead | BLENotify};

    // Mode characteristics (write)
    BLEIntCharacteristic mode_current_index_write_characteristic{"3f2b6f19-a5cd-44f1-8107-8d3e00635920", BLEWrite | BLEWriteWithoutResponse};
    BLECharacteristic mode_current_name_write_characteristic{"87b85c30-f154-4d7a-86b3-d0aa7dcc8a3d", BLEWrite | BLEWriteWithoutResponse, 16};
    BLEIntCharacteristic mode_current_throttle_enabled_write_characteristic{"f870d889-a14f-4fde-9a36-9828143fbf7e", BLEWrite | BLEWriteWithoutResponse};
    BLEIntCharacteristic mode_current_pas_enabled_write_characteristic{"2375bab6-13ad-45d9-879f-3bccf558f8d6", BLEWrite | BLEWriteWithoutResponse};
    BLEIntCharacteristic mode_current_dc_current_write_characteristic{"5cf2c276-865a-40e4-95ba-99e7addd2309", BLEWrite | BLEWriteWithoutResponse};
    BLEFloatCharacteristic mode_current_speed_limit_write_characteristic{"f7344d6d-00fb-4d27-b28b-afced2d0ea70", BLEWrite | BLEWriteWithoutResponse};

    ModeManager &modeManager;

    int convert_characteristic_value_to_int32(const uint8_t* value, int length);
    float convert_characteristic_value_to_float32(const uint8_t* value, int length);

};

#endif