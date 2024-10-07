/*! @file main.h
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include "Stepper.hpp"
#include "DS3231rtc.hpp"
// #include "SerialData.h"
#include "I2C_Slave.h"
#include "variable.h"

#define STEPS 2048

class MyProgram {
public:
  static MyProgram& instance() {
    static MyProgram instance;
    return instance;
  }

  static void setup() {
    instance().__setup__();
  }

  static void loop() {
    instance().__main__();
    // instance().__debug__();
  }

private:
  String __Timer1__, __Timer2__, __Timer3__;
  MyStepper stepper = MyStepper(
    STEPS,
    PINOUT_STEPPER[0],
    PINOUT_STEPPER[1],
    PINOUT_STEPPER[2],
    PINOUT_STEPPER[3]
  );
  DS3231rtc rtc;
  // SerialData serialData = SerialData(RX_TX, TX_RX);
  
  unsigned long LastTimeRTC = 0, LastTimeMonitor = 0;
  unsigned long LastTimeStepper1 = 0, LastTimeStepper2 = 0;
  bool stateStepper1 = false, stateStepper2 = false;

private:
  void __manualFeeder__(void);
  void __autoFeeder__(void);
  void __monitor__(void);

private:
  void __setup__(void);
  void __main__(void);
  void __debug__(void);
};