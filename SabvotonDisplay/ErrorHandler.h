#ifndef ErrorHandler_h
#define ErrorHandler_h

#include "Arduino.h"
#include "Definitions.h"
#include <cmsis_gcc.h> // CMSIS library for ARM Cortex-M systems

class ErrorHandler {
  public:
    //Functions
    static void raise_warning(String msg);
    static void raise_fatal_error(String msg);
    static void force_reset(void);
  private:
};

#endif