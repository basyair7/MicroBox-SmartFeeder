/*! @file BootButton.hpp
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <pushbutton.h>
#include "variable.h"

class BootButton {
public:
    BootButton(uint8_t pin, uint8_t mode) : bootBtn(pin, mode) { /*TODO(Not yet implemented)*/ }
    
    void begin() {
        bootBtn.init();
        this->wifiState = EEPROM.read(EEPROM_BLYNK_ADDR);
    }
    
    void WiFiMode() {
        this->currentButtonState = bootBtn.digitalReadPushButton();
        if (this->currentButtonState != this->lastButtonState)
            this->buttonChange = true;

        if (this->buttonChange) {
            if (this->currentButtonState == LOW) {
                this->wifiState = !this->wifiState;
                EEPROM.put(EEPROM_BLYNK_ADDR, this->wifiState);
                EEPROM.commit();
				delay(5);
                Serial.println(F("MODE BLYNK : "));
                Serial.println(this->wifiState ? "DISABLE" : "ENABLE");
                delay(2000);
                ESP.reset();
            }
            this->buttonChange = false;
        }
        this->lastButtonState = this->currentButtonState;
    }
    
private:
    PushButtonDigital bootBtn;
    bool currentButtonState = false;
    bool lastButtonState    = false;
    bool buttonChange       = false;
    bool wifiState          = false;
};
