#include "controllMoveBeltStrech.h"
#include <Arduino.h>

controllMoveBeltStrech::controllMoveBeltStrech(
    uint8_t belt_1_pin,
    uint8_t belt_2_pin,
    uint8_t strech_1_pin,
    uint8_t strech_2_pin ){
    _belt_1_pin = belt_1_pin;
    _belt_2_pin = belt_2_pin;
    _strech_1_pin = strech_1_pin;
    _strech_2_pin = strech_2_pin;

    pinMode(_belt_1_pin, OUTPUT);
    pinMode(_belt_2_pin, OUTPUT);
    pinMode(_strech_1_pin, OUTPUT);
    pinMode(_strech_2_pin, OUTPUT);
  }

  void controllMoveBeltStrech::forwardBelt(uint16_t time){
    startTime = millis();
    while(millis() - startTime <= time){
        digitalWrite(_belt_1_pin , HIGH);
        digitalWrite(_belt_2_pin , LOW);
    }
    digitalWrite(_belt_1_pin , LOW);
    digitalWrite(_belt_2_pin , LOW);
  }
  void controllMoveBeltStrech::backWardBelt(uint16_t time){
    startTime = millis();
    while(millis() - startTime <= time){
        digitalWrite(_belt_1_pin , LOW);
        digitalWrite(_belt_2_pin , HIGH);
    }
    digitalWrite(_belt_1_pin , LOW);
    digitalWrite(_belt_2_pin , LOW);
  }
  void controllMoveBeltStrech::openStrech(uint16_t time){
     startTime = millis();
    while(millis() - startTime <= time){
        digitalWrite(_strech_1_pin , LOW);
        digitalWrite(_strech_2_pin , HIGH);
    }
    digitalWrite(_strech_1_pin , LOW);
    digitalWrite(_strech_2_pin , LOW);
  }
  void controllMoveBeltStrech::closeStrech(uint16_t time){
     startTime = millis();
    while(millis() - startTime <= time){
        digitalWrite(_strech_1_pin , HIGH);
        digitalWrite(_strech_2_pin , LOW);
    }
    digitalWrite(_strech_1_pin , LOW);
    digitalWrite(_strech_2_pin , LOW);
  }