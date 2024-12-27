#ifndef Sabvoton_h
#define Sabvoton_h

#include "Arduino.h"
#include "ModbusMaster.h"
#include "Definitions.h"
#include "ErrorHandler.h"

class Sabvoton {
  public:
    //Conversion definitions
    static const float SINGLEFLOAT;
    static const float DOUBLEFLOAT;

    //Register definitions
    static const int Register_Init;
    static const int Register_Battery_Voltage;
    static const int Register_Motor_RPM;

    //Functions
    // Sabvoton(ModbusMaster node);
    void init(void);
    int read_register(int reg);
    float read_battery_voltage(void);
    float read_speed_mph(void);
    float read_speed_kph(void);
  private:
    ModbusMaster node;
};

#endif