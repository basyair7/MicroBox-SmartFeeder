/*! @file atmega328p_only.ino (program main)
 *  @version 1.0.0
*/

#include "Stepper.hpp"
#include "DS3231rtc.hpp"
#include "ultrasonic.h"
#include "variable.h"

MyStepper stepper = MyStepper(
  STEPS,
  PINOUT_STEPPER[0],
  PINOUT_STEPPER[1],
  PINOUT_STEPPER[2],
  PINOUT_STEPPER[3]
);

Ultrasonic hcsr04 = Ultrasonic(TRIG_PIN, ECHO_PIN);
DS3231rtc rtc;

unsigned long LastTimeGetDistance = 0, LastTimeStepper = 0;
unsigned long LastTimeLED = 0, LastTimeMonitor = 0;
bool stateStepper = false;
float distance; int capacity;

void __indikator__(void) {
  if ((unsigned long)(millis() - LastTimeLED) >= 100) {
    LastTimeLED = millis();

    if (capacity >= 0) {
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
    }
    else {
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
    }
  }
}

void __get_capacity__(void) {
  if ((unsigned long)(millis() - LastTimeGetDistance) >= 100) {
    LastTimeGetDistance = millis();

    distance = hcsr04.getDistance();
    capacity = map(distance, EMPTY + 1, FULL, 0, 100); // mapping
    capacity = (capacity <= 0 ? 0 : (capacity >= 100 ? 100 : capacity)); // filter 0-100
  }
}

void __monitor_data__(void) {
  if ((unsigned long) (millis() - LastTimeMonitor) >= 1000) {
    LastTimeMonitor = millis();

    Serial.print(F("Distance : "));
    Serial.print(distance);
    Serial.println(F("cm"));
    Serial.print(F("Capacity : "));
    Serial.print(capacity);
    Serial.println(F("%"));
    Serial.println();

    Serial.println(rtc.getDayOfWeek());
    Serial.print(F("Date : "));
    Serial.println(rtc.datestr());
    Serial.print(F("Time : "));
    Serial.println(rtc.timestr());

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  hcsr04.begin();
  stepper.setSpeed(SPEED_STEPPER);
  rtc.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  __indikator__();
  __get_capacity__();
  __monitor_data__();

  if (capacity >= 0) {
    for (const auto listTime : List_Time) {
      if (rtc.timestr() == listTime) {
        stateStepper = true;
        LastTimeStepper = millis();
      }
    }

    if (stateStepper && (unsigned long)(millis() - LastTimeStepper) <= 3000) {
      stepper.step(DEGRESS_STEPPER);
    }

    if (stateStepper && (unsigned long)(millis() - LastTimeStepper) > 6000 && (unsigned long)(millis() - LastTimeStepper) <= 9000)
    {
      stepper.step(-DEGRESS_STEPPER);
      stateStepper = false;
    }
  }

  delayMicroseconds(100);
}
