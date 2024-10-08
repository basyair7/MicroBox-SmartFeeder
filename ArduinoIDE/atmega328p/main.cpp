/*! @file main.cpp
 *  @version 1.0.0
*/

#include "main.h"

// String Timer1, Timer2, Timer3;
bool auto_state, switch_state;
int capacity;

void MyProgram::__autoFeeder__(void) {
  for (const auto listTime : ListTime) {
    if ((unsigned long) (millis() - this->LastTimeRTC) >= 1000) {
      this->LastTimeRTC = millis();
      if (this->rtc.timestr() == listTime) {
        this->stateStepper1 = true;
        this->LastTimeStepper1 = millis();
      }
    }
  }

  if (this->stateStepper1) {
    /*
    for (int i = 0; i < 4; i++) {
      if ((unsigned long)(millis() - this->LastTimeStepper1) <= 3000) {
        this->stepper.step(DEGRESS_STEPPER);
      }

      if ((unsigned long)(millis() - this->LastTimeStepper1) > 6000 && (unsigned long)(millis() - this->LastTimeStepper1) <= 9000)
      {
        this->stepper.step(-DEGRESS_STEPPER);
      }

      this->stateStepper1 = (i == 3 ? false : true);
    }
    */

    if ((unsigned long)(millis() - this->LastTimeStepper1) <= 3000) {
      this->stepper.step(DEGRESS_STEPPER);
    }

    if ((unsigned long)(millis() - this->LastTimeStepper1) > 6000 && (unsigned long)(millis() - this->LastTimeStepper1) <= 9000)
    {
      this->stepper.step(-DEGRESS_STEPPER);
      this->stateStepper1 = false;
    }
  }
}

void MyProgram::__manualFeeder__(void) {
  if (this->stateStepper2 && (unsigned long)(millis() - this->LastTimeStepper2) <= 3000)
  {
    this->stepper.step(DEGRESS_STEPPER);
  }

  if (this->stateStepper2 && (unsigned long)(millis() - this->LastTimeStepper2) > 6000 && (unsigned long)(millis() - this->LastTimeStepper2) <= 9000)
  {
    this->stepper.step(-DEGRESS_STEPPER);
    this->stateStepper2 = false;
  }
}

void MyProgram::__monitor__(void) {
  if ((unsigned long)(millis() - this->LastTimeMonitor) >= 1000) {
    this->LastTimeMonitor = millis();

    Serial.print(F("AUTO : "));
    Serial.println(auto_state ? F("Enable") : F("Disable"));
    Serial.print(F("Switch : "));
    Serial.println(switch_state ? F("Open") : F("Close"));

    Serial.println();
    Serial.print(F("Capacity : "));
    Serial.print(capacity);
    Serial.println(F("%"));
    
    Serial.println();
    Serial.println(this->rtc.getDayOfWeek());
    Serial.print(F("Date : "));
    Serial.println(this->rtc.datestr());
    Serial.print(F("Time : "));
    Serial.println(this->rtc.timestr());
  }
}

void MyProgram::__setup__(void) {
  Serial.begin(9600);
  // serialData.begin(9600);
  I2CSlave_Init(I2C_MASTER_ADDR, JSONSIZE);

  this->stepper.setSpeed(SPEED_STEPPER);
  this->rtc.begin();
}

void MyProgram::__main__(void) {
  // get data from nodemcu
  I2CSlave_getData(&capacity, &auto_state, &switch_state);
  this->__monitor__();

  if (capacity > 0) {
    if (auto_state) {
      this->__autoFeeder__();
    }

    if (!auto_state && switch_state) {
      this->LastTimeStepper2 = millis();
      this->stateStepper2 = true;
      this->__manualFeeder__();
    }
  }
  
  delayMicroseconds(50);
}

void MyProgram::__debug__() {
  // get data from nodemcu
  // serialData.reqData();
  // serialData.getData(50);
  // serialData.getBlynkCmd(&auto_state, &switch_state);
  // serialData.getTimeAuto(&Timer1, &Timer2, &Timer3);
  // capacity = serialData.getCapacity();

  I2CSlave_getData(&capacity, &auto_state, &switch_state);

  this->__monitor__();

  delayMicroseconds(50);
}
