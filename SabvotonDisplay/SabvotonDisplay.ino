#include <ModbusMaster.h>
#include <ArduinoBLE.h>
#include <string.h>
#include "Sabvoton.h"
#include "Bluetooth.h"
#include "ErrorHandler.h"
#include "Definitions.h"

// ModbusMaster node;
Sabvoton sabvoton;
Bluetooth bluetooth;

unsigned long lastMillis = 0;

void setup() {
  //Initialize Sabvoton serial
  Serial1.begin(19200, SERIAL_8O1);
  //Initialize debug serial
  Serial.begin(115200);

  //Initialize Sabvoton controller
  sabvoton.init();
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

  //Every ~500ms get new readings for all registers (change SABVOTON_READ_INTERVAL_MS to adjust interval)
  if(millis() > (lastMillis + SABVOTON_READ_INTERVAL_MS)) {
    float batteryVoltage = sabvoton.read_battery_voltage();
    int speed;
    if(SPEED_UNIT == 1) {
      speed = sabvoton.read_speed_mph();
    } else if(SPEED_UNIT == 0) {
      speed = sabvoton.read_speed_kph();
    }
    
    Serial.print("Battery voltage: ");
    Serial.println(batteryVoltage);
    Serial.print("Speed: ");
    Serial.println(speed);

    //send the new readings over BLE
    bluetooth.write_battery_voltage(batteryVoltage);
    bluetooth.write_speed(speed);

    lastMillis = millis();
  }
}
