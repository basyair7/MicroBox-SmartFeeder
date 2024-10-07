/*! @file I2C_Master.cpp
 *  @version 1.0.0
*/

#include "I2C_Master.h"

void I2C_Master::begin(uint8_t __SDA__, uint8_t __SCL__) {
    Wire.begin(__SDA__, __SCL__);
}

void I2C_Master::setCapacity(float capacity) {
    this->__capacity__ = capacity;
}

void I2C_Master::setBlynkCmd(bool auto_state, bool switch_state) {
    this->__auto_state__   = auto_state;
    this->__switch_state__ = switch_state;
}

void I2C_Master::runSendData(long __delay__) {
    if ((unsigned long)(millis() - this->LastTimeSendData) >= (unsigned long)__delay__) {
        this->LastTimeSendData = millis();

        DynamicJsonDocument data(this->__jsonSize__);

        data["c"] = this->__capacity__;
        data["a"] = this->__auto_state__;
        data["m"] = this->__switch_state__;

        String jsonBuffer;
        size_t n = serializeJson(data, jsonBuffer);

        Wire.beginTransmission(this->__address__);
        Wire.write(jsonBuffer.c_str(), n);
        Wire.endTransmission();
    }
}