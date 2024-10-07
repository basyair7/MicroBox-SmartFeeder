/*! @file SerialData.cpp
 *  @version 1.0.0
*/

#include "SerialData.h"
#include <ArduinoJson.h>

void SerialData::begin(uint32_t baudRate) {
    this->__mySerial__.begin(baudRate);
}

void SerialData::setCapacity(float capacity) {
    this->__capacity__ = capacity;
}

void SerialData::setBlynkCmd(bool auto_state, bool switch_state)
{
    this->__auto_state__   = auto_state;
    this->__switch_state__ = switch_state;
}

void SerialData::setTimeAuto(const String Timer1, const String Timer2, const String Timer3)
{
	this->__Timer1__ = Timer1;
	this->__Timer2__ = Timer2;
	this->__Timer3__ = Timer3;
}

void SerialData::runSendData(long __delay__) {
    if ((unsigned long) (millis() - this->LastSendData) >= (unsigned long)__delay__) {
        this->LastSendData = millis();
        StaticJsonDocument<500> data;

        DeserializationError err = deserializeJson(data, this->__mySerial__);
        if (!err) {
            uint8_t code_reqData = data["reqdata"];
            if (code_reqData) {
                data["capacity"]         = this->__capacity__;
                data["blynk"]["auto"]    = this->__auto_state__;
                data["blynk"]["switch"]  = this->__switch_state__;
                data["timers"]["timer1"] = this->__Timer1__;
                data["timers"]["timer2"] = this->__Timer2__;
                data["timers"]["timer3"] = this->__Timer3__;

                serializeJson(data, this->__mySerial__);
            }
        }
        else {
            Serial.print(F("Error Deserialization : "));
            Serial.println(err.c_str());
        }
    }
}