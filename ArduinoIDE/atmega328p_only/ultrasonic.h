/*! @file ultrasonic.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>

class Ultrasonic {
public:
  Ultrasonic(uint8_t TriggerPin, uint8_t EchoPin) {
    this->__TriggerPin__ = TriggerPin;
    this->__EchoPin__    = EchoPin;
  }

  void begin() {
    pinMode(this->__TriggerPin__, OUTPUT);
    pinMode(this->__EchoPin__, INPUT);
  }

  float getDistance() {
    digitalWrite(this->__TriggerPin__, LOW);
    delayMicroseconds(2);
    digitalWrite(this->__TriggerPin__, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->__TriggerPin__, LOW);

    return pulseIn(this->__EchoPin__, HIGH);
  }

private:
  uint8_t __TriggerPin__, __EchoPin__;
};
