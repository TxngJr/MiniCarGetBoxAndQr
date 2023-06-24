#ifndef CONTROLLHAND_H
#define CONTROLLHAND_H

#include <Arduino.h>
#include <Servo.h>

class controllHand {
private:
  uint8_t _servo_1_pin;
  uint8_t _servo_2_pin;
  uint8_t _servo_3_pin;
  Servo servo1;
  Servo servo2;
  Servo servo3;
public:
  controllHand(
    uint8_t servo_1_pin,
    uint8_t servo_2_pin,
    uint8_t servo_3_pin);
  void getBox();
  void readyGetBox();
  void dropBox();
  void readyDropBox();
};

#endif