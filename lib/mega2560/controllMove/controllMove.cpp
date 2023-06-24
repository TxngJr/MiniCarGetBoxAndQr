#include "controllMove.h"

controllMoveMotor::controllMoveMotor(uint8_t font_left_1_pin,
                                     uint8_t font_left_2_pin,
                                     uint8_t font_right_1_pin,
                                     uint8_t font_right_2_pin,
                                     uint8_t back_left_1_pin,
                                     uint8_t back_left_2_pin,
                                     uint8_t back_right_1_pin,
                                     uint8_t back_right_2_pin) {
  FONT_LEFT_1_PIN = font_left_1_pin;
  FONT_LEFT_2_PIN = font_left_2_pin;
  FONT_RIGHT_1_PIN = font_right_1_pin;
  FONT_RIGHT_2_PIN = font_right_2_pin;
  BACK_LEFT_1_PIN = back_left_1_pin;
  BACK_LEFT_2_PIN = back_left_2_pin;
  BACK_RIGHT_1_PIN = back_right_1_pin;
  BACK_RIGHT_2_PIN = back_right_2_pin;

  pinMode(FONT_LEFT_1_PIN, OUTPUT);
  pinMode(FONT_LEFT_2_PIN, OUTPUT);
  pinMode(FONT_RIGHT_1_PIN, OUTPUT);
  pinMode(FONT_RIGHT_2_PIN, OUTPUT);
  pinMode(BACK_LEFT_1_PIN, OUTPUT);
  pinMode(BACK_LEFT_2_PIN, OUTPUT);
  pinMode(BACK_RIGHT_1_PIN, OUTPUT);
  pinMode(BACK_RIGHT_2_PIN, OUTPUT);

  digitalWrite(FONT_LEFT_1_PIN, LOW);
  digitalWrite(FONT_LEFT_2_PIN, LOW);
  digitalWrite(FONT_RIGHT_1_PIN, LOW);
  digitalWrite(FONT_RIGHT_2_PIN, LOW);
  digitalWrite(BACK_LEFT_1_PIN, LOW);
  digitalWrite(BACK_LEFT_2_PIN, LOW);
  digitalWrite(BACK_RIGHT_1_PIN, LOW);
  digitalWrite(BACK_RIGHT_2_PIN, LOW);
}

void controllMoveMotor::forward(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}

void controllMoveMotor::moveLeft(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}
void controllMoveMotor::rotateLeft(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}
void controllMoveMotor::forwardLeftOblique(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);
}
void controllMoveMotor::moveRight(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}
void controllMoveMotor::rotateRight(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}
void controllMoveMotor::forwardRightOblique(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}
void controllMoveMotor::back(int16_t speed) {
  if (speed > 255) {
    speed = 255;
  } else if (speed < 0) {
    return;
  }

  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}