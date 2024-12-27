#include "Sabvoton.h"

//Conversion definitions
const float Sabvoton::SINGLEFLOAT = 54.6;
const float Sabvoton::DOUBLEFLOAT = 6553.0;

//Register definitions
const int Sabvoton::Register_Init = 4039;
const int Sabvoton::Register_Battery_Voltage = 2749;
const int Sabvoton::Register_Motor_RPM = 2757;

void Sabvoton::init(void) {
  //Initialize modbus
  this->node.begin(1, Serial1);

  uint8_t result;

  result = this->node.writeSingleRegister(Sabvoton::Register_Init, 13345);
  delay(50);
  result = this->node.writeSingleRegister(Sabvoton::Register_Init, 13345);
  delay(50);
  result = this->node.writeSingleRegister(Sabvoton::Register_Init, 13345);
  delay(50);

  if(result != node.ku8MBSuccess) {
    // ErrorHandler::raise_fatal_error("Failed to initialize Sabvoton controller");
    // return;
  }
  Serial.println("Sabvoton controller initialized");
}

int Sabvoton::read_register(int reg) {
  int result = node.readHoldingRegisters(reg, 1);
  if(result != node.ku8MBSuccess) {
    ErrorHandler::raise_warning("Failed to read register: " + String(reg));
    return -99999;
  }
  return node.getResponseBuffer(0);
}

float Sabvoton::read_battery_voltage(void) {
  int result = this->read_register(Sabvoton::Register_Battery_Voltage);
  if(result == -99999) {
    return 84.32;
  }
  return (float) ((float)result) / SINGLEFLOAT;
}

float Sabvoton::read_speed_mph(void) {
  int result = this->read_register(Sabvoton::Register_Motor_RPM);
  if(result == -99999) {
    return 15.5;
  }
  return (float) ((result * WHEEL_CIRCUMFERENCE) / 1056);
}

float Sabvoton::read_speed_kph(void) {
  int result = this->read_register(Sabvoton::Register_Motor_RPM);
  if(result == -99999) {
    return 15.5;
  }
  return (float) ((result * WHEEL_CIRCUMFERENCE) / 656.2);
}