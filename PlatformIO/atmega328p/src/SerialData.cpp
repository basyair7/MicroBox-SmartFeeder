#include "SerialData.h"
#include "ArduinoJson.h"

void SerialData::getData(long __delay__) {
    if ((unsigned long) (millis() - this->LastTimeGetData) >= (unsigned long) __delay__)
    {
        this->LastTimeGetData = millis();
        StaticJsonDocument<500> data;
        DeserializationError error = deserializeJson(data, myserial);

        if (!error) {
            data["distance"]        = this->__distance__;
            data["blynk"]["auto"]   = this->__auto__;
            data["blynk"]["switch"] = this->__switch__;
            
        }
    }
}