#ifndef CONTROLLMOVEBELT_STRECH_H
#define CONTROLLMOVEBELT_STRECH_H

#include <Arduino.h>

class controllMoveBeltStrech {
private:
  uint8_t _belt_1_pin;
  uint8_t _belt_2_pin;
  uint8_t _strech_1_pin;
  uint8_t _strech_2_pin;
  unsigned long startTime = millis();
public:
  controllMoveBeltStrech(
    uint8_t belt_1_pin,
  uint8_t belt_2_pin,
  uint8_t strech_1_pin,
  uint8_t strech_2_pin);

  void forwardBelt(uint16_t time);
  void backWardBelt(uint16_t time);
  void openStrech(uint16_t time);
  void closeStrech(uint16_t time);
};

#endif