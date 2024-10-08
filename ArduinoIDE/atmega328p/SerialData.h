/*! @file SerialData.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

class SerialData {
public:
  /* Requitment :
    * RX : digital GPIO arduino
    * TX : digital GPIO arduino
    * JsonSize : 50, 200, 500, 1024
  */
  SerialData(uint8_t __rx__, uint8_t __tx__, uint16_t jsonSize) : __mySerial__(__rx__, __tx__), __jsonSize__(jsonSize) {
    /*TODO(Not yet implemented)*/ 
  }
  
  void begin(uint32_t baudRate) {
    this->__mySerial__.begin(baudRate);
  }

  void reqData() {
    DynamicJsonDocument data(this->__jsonSize__);
    data["reqdata"] = 1;
    serializeJson(data, this->__mySerial__);
  }

  void getTimeAuto(String* Timer1, String* Timer2, String* Timer3) {
    *Timer1 = this->__Timer1__;
    *Timer2 = this->__Timer2__;
    *Timer3 = this->__Timer3__;
  }

  void getBlynkCmd(bool* auto_state, bool* switch_state) {
    *auto_state   = this->__auto_state__;
    *switch_state = this->__switch_state__;
  }

  int getCapacity() {
    return this->__capacity__;
  }

  void getData(long __delay__) {
    if ((unsigned long)(millis() - this->LastTimeGetData) >= (unsigned long)__delay__) {
      this->LastTimeGetData = millis();
      DynamicJsonDocument data(this->__jsonSize__);

      DeserializationError err = deserializeJson(data, this->__mySerial__);
      if (!err) {
        uint8_t code_getData = data["getdata"];
        if (code_getData) {          
          this->__capacity__     = data["capacity"];
          this->__auto_state__   = data["blynk"]["auto"];
          this->__switch_state__ = data["blynk"]["switch"];

          this->__Timer1__ = data["timers"]["timer1"].as<String>();
          this->__Timer2__ = data["timers"]["timer2"].as<String>();
          this->__Timer3__ = data["timers"]["timer3"].as<String>();
        }
      }
      else {
        Serial.print(F("Error Deserialization : "));
        Serial.println(err.c_str());
      }
    }
  }

private:
  String __Timer1__, __Timer2__, __Timer3__;
  int __capacity__;
  bool __auto_state__, __switch_state__;
  unsigned long LastTimeGetData = 0;

private:
  uint16_t __jsonSize__;
  SoftwareSerial __mySerial__;
};
