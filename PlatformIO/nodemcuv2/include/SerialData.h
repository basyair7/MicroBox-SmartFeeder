/*! @file SerialData.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

class SerialData {
public:
    SerialData(uint8_t rx, uint8_t tx) : __mySerial__(rx, tx) { /*TODO(Not yet implemented)*/ }
    
    void begin(uint32_t baudRate);

    void setCapacity(float capacity);

    void setBlynkCmd(bool auto_state, bool switch_state);
	
	void setTimeAuto(const String Timer1, const String Timer2, const String Timer3);

    void runSendData(long __delay__);

private:
    String __Timer1__ = "7:0:0", __Timer2__ = "12:0:0", __Timer3__ = "17:0:0";
    float __capacity__;
    bool __auto_state__, __switch_state__;
	unsigned long LastSendData = 0;
    
private:
    SoftwareSerial __mySerial__;
};