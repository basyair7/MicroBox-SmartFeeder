/*! @file I2C_Slave.cpp
 *  @version 1.0.0
*/

#include "I2C_Slave.h"

bool __auto_state__, __switch_state__;
int __capacity__;
uint16_t __jsonSize__;

void I2CSlave_getData(int* capacity, bool* auto_state, bool* switch_state) {
  *auto_state   = __auto_state__;
  *switch_state = __switch_state__;
  *capacity     = __capacity__;
}

void receiveEvent(int bytes) {
  String raw_data;
  while (Wire.available()) {
    char c = Wire.read();
    raw_data += c;
  }

  DynamicJsonDocument data(__jsonSize__);
  DeserializationError err = deserializeJson(data, raw_data);
  if (!err) {
    __auto_state__   = data["a"];
    __switch_state__ = data["m"];
    __capacity__     = data["c"];
  }
  else {
    Serial.print(F("Deserialize Json Error : "));
    Serial.println(err.c_str());
  }
}

void I2CSlave_Init(uint8_t address, uint16_t jsonSize) {
  __jsonSize__ = jsonSize;
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
}
