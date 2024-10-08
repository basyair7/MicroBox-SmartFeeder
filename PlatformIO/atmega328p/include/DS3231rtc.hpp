/*! @file DS3231rtc.hpp
 *  @version 1.0.0
*/

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <RTClib.h>

class DS3231rtc {
public:
    void begin() {
        while (!this->__rtc__.begin()) {
            Serial.println(F("Status : Couldn't find RTC"));
            delay(100);
        }

        if (this->__rtc__.lostPower()) {
            Serial.println(F("Status : RTC Lost Power, setting the time!"));
            this->__rtc__.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }

    DateTime DSnow() {
        return this->__rtc__.now();
    }

    // function to manual adjust RTC time
    void manualAdjust(
        uint8_t month, uint8_t day, uint16_t year,
        uint8_t hour, uint8_t minute, uint8_t second
    ) {
        this->__rtc__.adjust(DateTime(year, month, day, hour, minute, second));
    }

    // function to automatically adjust RTC time
    void autoAdjust() {
        this->__rtc__.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // function to get the current date
    template <typename T> void date(T* month, T* day, T* year) {
        this->__now__ = this->__rtc__.now();
        if (month != nullptr) *month = this->__now__.month();
        if (day != nullptr)   *day   = this->__now__.day();
        if (year != nullptr)  *year  = this->__now__.year();
    }

    // function to get the current time
    template <typename T> void time(T* hour, T* minute, T* second) {
        this->__now__ = this->__rtc__.now();
        if (hour != nullptr)   *hour   = this->__now__.hour();
        if (minute != nullptr) *minute = this->__now__.minute();
        if (second != nullptr) *second = this->__now__.second();
    }

    // function to get the date as a string
    String datestr(void) {
        return String(this->__rtc__.now().month()) + "/" + String(this->__rtc__.now().day()) + "/" + String(this->__rtc__.now().year());
    }

    // function to get the time as string
    String timestr(void) {
        return String(this->__rtc__.now().hour()) + ":" + String(this->__rtc__.now().minute()) + ":" + String(this->__rtc__.now().second());
    }

    // function to get day of week
    String getDayOfWeek(void) {
        return this->listDayOfWeek[this->__rtc__.now().dayOfTheWeek()];
    }

private:
    char listDayOfWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

private:
    RTC_DS3231 __rtc__;
    DateTime __now__;
};
