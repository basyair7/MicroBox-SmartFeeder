/*! @file SerialData.h
 *  @version 1.0.0
*/

#include "Arduino.h"
#include "SoftwareSerial.h"

class SerialData {
public:
    SerialData(uint8_t __rx__, uint8_t __tx__) : myserial(__rx__, __tx__) { /*TODO(Not yet implement)*/ }
    
    void getData(long __delay__);

public:
    float __distance__;
    bool __auto__, __switch__;
    
private:
    SoftwareSerial myserial;
    unsigned long LastTimeGetData = 0;
};