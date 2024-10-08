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
        /*TODO(Not yet implement)*/ 
    }

    void begin(uint32_t baudRate) {
        this->__mySerial__.begin(baudRate);
    }

    void reqData();
    void getTimeAuto(String* Timer1, String* Timer2, String* Timer3);
    void getBlynkCmd(bool* auto_state, bool* switch_state);
    int getCapacity();
    
    void getData(long __delay__);

public:
    String __Timer1__, __Timer2__, __Timer3__;
    int __capacity__;
    bool __auto_state__, __switch_state__;
    
private:
    SoftwareSerial __mySerial__;
    uint16_t __jsonSize__;
    unsigned long LastTimeGetData = 0;
};
