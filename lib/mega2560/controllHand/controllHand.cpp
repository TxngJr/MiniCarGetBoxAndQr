#include "controllHand.h"
#include <Servo.h>
// 1 หัว 2 กลาง 3 แกน
// 3 ready 150 2 65
controllHand::controllHand(
  uint8_t servo_1_pin,
  uint8_t servo_2_pin,
  uint8_t servo_3_pin) {
  _servo_1_pin = servo_1_pin;
  _servo_2_pin = servo_2_pin;
  _servo_3_pin = servo_3_pin;
  servo1.attach(_servo_1_pin);
  servo2.attach(_servo_2_pin);
  servo3.attach(_servo_3_pin);
}
void controllHand::getBox() {
  servo1.write(25);
  delay(500);
  servo2.write(168);
  delay(500);
  servo3.write(92);
  delay(500);
  servo1.write(110);
  delay(500);
}
void controllHand::readyGetBox() {
  servo3.write(160);
  delay(500);
  servo2.write(75);
  delay(500);
  servo1.write(110);
  delay(500);
}
void controllHand::dropBox() {
  servo1.write(25);
  delay(500);
  servo2.write(15);
  delay(500);
  servo3.write(88);
  delay(500);
  servo1.write(110);
  delay(500);
}
void controllHand::readyDropBox() {
  servo3.write(92);
  delay(500);
  servo2.write(168);
  delay(500);
  servo1.write(110);
  delay(500);
}
