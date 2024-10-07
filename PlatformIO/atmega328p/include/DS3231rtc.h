/*! @file DS3231rtc.h
 *  @version 1.0.0
*/

#include "Arduino.h"
#include "SPI.h"
#include "RTClib.h"

#pragma once

class DS3231rtc {
public:
  void begin() {
    if (!_rtc.begin()) {
      Serial.println(F("Status : Couldn't find RTC"));
      while(1);
    }

    if (_rtc.lostPower()) {
      Serial.println(F("RTC Lost Power, setting the time!"));
      _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }

  DateTime DSnow() {
    return _rtc.now();
  }

  void manualAdjust(
    uint8_t month, uint8_t day, uint16_t year, 
    uint8_t hour, uint8_t minute, uint8_t second)
  {
    _rtc.adjust(DateTime(year, month, day, hour, minute, second));
  }

  // function to automatically adjust RTC time
  void autoAdjust()  {
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // function to get the current date
  template <typename T> void date(T* month, T* day, T* year) {
    this->now = _rtc.now();
    if (month != nullptr) *month = now.month();
    if (day != nullptr)   *day   = now.day();
    if (year != nullptr)  *year  = now.year();
  }


  // function to get the current time
  template <typename T> void time(T* hour, T* minute, T* second) {
    this->now = _rtc.now();
    if(hour != nullptr)   *hour   = now.hour();
    if(minute != nullptr) *minute = now.minute();
    if(second != nullptr) *second = now.second();
  }

  // function to get day of week
  String getDayOfWeek(void) {
    return this->listDayOfWeek[_rtc.now().dayOfTheWeek()];
  }

  // function to get the date as a string
  String datestr(void)  {
    return String(_rtc.now().month()) + "/" + String(_rtc.now().day()) + "/" + String(_rtc.now().year());
  }
  
  // function to get the time as a string
  String timestr(void)  {
    return String(_rtc.now().hour()) + ":" + String(_rtc.now().minute()) + ":" + String(_rtc.now().second());
  }

private:
  char listDayOfWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  RTC_DS3231 _rtc; // RTC object
  DateTime now; // DateTime object
};