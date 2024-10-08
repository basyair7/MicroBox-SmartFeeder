/*! @file SerialData.cpp
 *  @version 1.0.0
*/

#include "SerialData.h"

void SerialData::reqData() {
    DynamicJsonDocument data(this->__jsonSize__);
    data["reqdata"] = 1;
    serializeJson(data, this->__mySerial__);
}

void SerialData::getTimeAuto(String* Timer1, String* Timer2, String* Timer3) {
    *Timer1 = this->__Timer1__;
    *Timer2 = this->__Timer2__;
    *Timer3 = this->__Timer3__;
}

void SerialData::getBlynkCmd(bool* auto_state, bool* switch_state) {
    *auto_state   = this->__auto_state__;
    *switch_state = this->__switch_state__;
}

int SerialData::getCapacity() {
    return this->__capacity__;
}

void SerialData::getData(long __delay__) {
    if ((unsigned long) (millis() - this->LastTimeGetData) >= (unsigned long) __delay__)
    {
        this->LastTimeGetData = millis();
        StaticJsonDocument<500> data;
        DeserializationError error = deserializeJson(data, this->__mySerial__);

        if (!error) {
            uint8_t code_getData = data["getdata"];
            if (code_getData) {
                this->__capacity__      = data["capacity"];
                this->__auto_state__    = data["blynk"]["auto"];
                this->__switch_state__  = data["blynk"]["switch"];

                this->__Timer1__ = data["timers"]["timer1"].as<String>();
                this->__Timer2__ = data["timers"]["timer2"].as<String>();
                this->__Timer3__ = data["timers"]["timer3"].as<String>();
            }
        }
        else {
            Serial.print(F("Error Deserialization : "));
            Serial.println(error.c_str());
        }
    }
}
