#include "Mode.h"

void Mode::serialize(uint8_t *buffer) const {
  int bufCount = 0;

  memcpy(buffer, name, sizeof(name));
  bufCount += sizeof(name);

  buffer[bufCount] = throttleEnabled ? 1 : 0;
  bufCount += sizeof(uint8_t);
  
  buffer[bufCount] = pasEnabled ? 1 : 0;
  bufCount += sizeof(uint8_t);

  memcpy(buffer + bufCount, &dcCurrent, sizeof(dcCurrent));
  bufCount += sizeof(dcCurrent);

  memcpy(buffer + bufCount, &speedLimit, sizeof(speedLimit));
  bufCount += sizeof(speedLimit);
}

void Mode::deserialize(const uint8_t *buffer) {
  int bufCount = 0;

  memcpy(&name, buffer, sizeof(name));
  bufCount += sizeof(name);

  throttleEnabled = buffer[bufCount] != 0;
  bufCount += sizeof(throttleEnabled);

  pasEnabled = buffer[bufCount] != 0;
  bufCount += sizeof(pasEnabled);

  memcpy(&dcCurrent, buffer + bufCount, sizeof(dcCurrent));
  bufCount += sizeof(dcCurrent);

  memcpy(&speedLimit, buffer + bufCount, sizeof(speedLimit));
  bufCount += sizeof(speedLimit);
}
