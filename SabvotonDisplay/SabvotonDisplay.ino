#include <ModbusMaster.h>
#include <ArduinoBLE.h>
#include <string.h>
#include "Sabvoton.h"
#include "Bluetooth.h"
#include "Mode.h"
#include "ModeManager.h"
#include "Storage.h"
#include "Throttle.h" 
#include "ErrorHandler.h"
#include "Definitions.h"

// ModbusMaster node;
Sabvoton sabvoton;
ModeManager modeManager;
Bluetooth bluetooth(modeManager);
Storage storage;

unsigned long lastMillis = 0;

void setup() {
  //Initialize Sabvoton serial
  Serial1.begin(19200, SERIAL_8O1);

  //Initialize debug serial
  Serial.begin(115200);

  //Initialize throttle / PAS pins (if enabled)
  if(THROTTLE_PASSTHROUGH_ENABLED || PAS_PASSTHROUGH_ENABLED) {
    pinMode(THROTTLE_OUT_PIN, OUTPUT);
    analogWriteResolution(8);
    analogWrite(THROTTLE_OUT_PIN, 0); //Immediately setting the pin to 0V since this is wired directly to the controller's throttle and it scares me
  }
  if(THROTTLE_PASSTHROUGH_ENABLED) {
    pinMode(THROTTLE_IN_PIN, INPUT_PULLDOWN);
  }
  if(PAS_PASSTHROUGH_ENABLED) {
    pinMode(PAS_IN_PIN, INPUT_PULLDOWN);
  }

  //Initialize Sabvoton controller
  sabvoton.init();
  delay(100);

  //Load all modes stored in EEPROM
  modeManager.loadModes();
  if(modeManager.getModeCount() > 0) {
    //If there are any stored modes, default to the first mode (this should be your road-legal mode)
    modeManager.current_mode_index = 0;
  }
  delay(100);

  //Initialize BLE
  bluetooth.init();
  delay(100);

  Serial.println("Sabvoton Display initialized");
}

void loop() {
  //Maintain BLE connection (supposedly - I have no idea)
  bluetooth.poll();

  //Handle millis() looping back around (happens after 50 days of run-time according to documentation)
  if(millis() < lastMillis && lastMillis != 0) {
    lastMillis = millis();
  }

  // Check for any new data written to BLE characteristics
  for(int characteristicIndex = 0; characteristicIndex < (sizeof(bluetooth.characteristics) / sizeof(Bluetooth::BLECharacteristicInfo)); characteristicIndex++) {
    if(!bluetooth.characteristics[characteristicIndex].isWrite) {
      continue;
    }
    if(bluetooth.characteristics[characteristicIndex].characteristic->written()) {
      bluetooth.handle_write(bluetooth.characteristics[characteristicIndex].characteristic);
    }
  }

  // Every ~500ms get new readings for all registers (change SABVOTON_READ_INTERVAL_MS to adjust interval)
  if(millis() > (lastMillis + SABVOTON_READ_INTERVAL_MS)) {
    float batteryVoltage = sabvoton.read_battery_voltage();
    float speed;
    if(SPEED_UNIT == 1) {
      speed = sabvoton.read_speed_mph();
    } else if(SPEED_UNIT == 0) {
      speed = sabvoton.read_speed_kph();
    }
    
    // Serial.print("Battery voltage: ");
    // Serial.println(batteryVoltage);
    // Serial.print("Speed: ");
    // Serial.println(speed);

    //send the new readings over BLE
    bluetooth.write_battery_voltage(batteryVoltage);
    bluetooth.write_speed(speed);
    bluetooth.write_current_mode();

    lastMillis = millis();
  }

  // Check for changes to the current mode and send any updates to the app
  // if(modeManager.current_mode_updated) {
  //   bluetooth.write_current_mode();
  //   modeManager.current_mode_updated = false;
  // }
}
