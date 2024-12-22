#include "ErrorHandler.h"

void ErrorHandler::raise_warning(String msg) {
  Serial.print("[Warning] ");
  Serial.println(msg);
}

void ErrorHandler::raise_fatal_error(String msg) {
  Serial.print("[Fatal Error] ");
  Serial.println(msg);
  ErrorHandler::force_reset();
}

void ErrorHandler::force_reset(void) {
  Serial.println("Fatal error detected - resetting in 500ms\n");
  delay(500);
  // Perform a system reset using the NVIC system control block
  NVIC_SystemReset(); // Triggers a full system reset
}