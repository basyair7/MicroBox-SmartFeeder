/*! @file I2C_Master.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

class I2C_Master {
public:
  I2C_Master(uint8_t slaveAddress, uint16_t jsonSize) : 
    __address__(slaveAddress), __jsonSize__(jsonSize) 
  {
    /*TODO(Not yet implemented)*/ 
  }

  void begin(uint8_t __SDA__, uint8_t __SCL__) {
    Wire.begin(__SDA__, __SCL__);
  }

  void setCapacity(float capacity) {
    this->__capacity__ = capacity;
  }

  void setBlynkCmd(bool auto_state, bool switch_state) {
    this->__auto_state__   = auto_state;
    this->__switch_state__ = switch_state;
  }

  void runSendData(long __delay__) {
    if ((unsigned long)(millis() - this->LastTimeSendData) >= (unsigned long)__delay__) {
      this->LastTimeSendData = millis();
      DynamicJsonDocument data(this->__jsonSize__);

      data["c"] = this->__capacity__;
      data["a"] = this->__auto_state__;
      data["m"] = this->__switch_state__;

      String jsonBuffer;
      size_t n = serializeJson(data, jsonBuffer);

      Wire.beginTransmission(__address__);
      Wire.write(jsonBuffer.c_str(), n);
      Wire.endTransmission();
    }
  }

private:
  int __capacity__;
  bool __auto_state__, __switch_state__;
  unsigned long LastTimeSendData = 0;

private:
  uint8_t __address__;
  uint16_t __jsonSize__;
};