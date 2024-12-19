/** 
 *  @file DS3231rtc.hpp
 *  @version 1.2.0
 *  @date 2024
 *  @author basyair7
 * 
 *  @copyright
 *  Copyright (C) 2024, basyair7
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <RTClib.h>

/**
 * @class DS3231rtc
 * @brief A wrapperclass for managing the DS3231 RTC module using RTClib.
 */
class DS3231rtc : public RTC_DS3231 {
    public:
        bool twelve_hour_format_flag;

        /**
         * @brief Initialize the DS3231 RTC module.
         * 
         * This function attempts to initialize the RTC module. If the module is not 
         * found, it continuously retries with a delay. Additionally, if the RTC has
         * lost power, it resets the time to the compile time.
         * 
         * @param twelve_hour_format_flag
         */
        void begin(bool twelve_hour_format_flag = false) {
            while (!RTC_DS3231::begin()) {
                Serial.println(F("Status : Couldn't find RTC"));
                delay(100);
            }

            if (RTC_DS3231::lostPower()) {
                Serial.println(F("Status : RTC Lost Power, setting the time!"));
                RTC_DS3231::adjust(DateTime(F(__DATE__), F(__TIME__)));
            }

            this->twelve_hour_format_flag = twelve_hour_format_flag;
            Serial.println();
            Serial.println(F("DS3231rtc Initializing..."));
            Serial.printf("Date : %s\nTime : %s\n\n\n", this->datestr().c_str(), this->timestr().c_str());
        }

        /**
         * @brief Get the current date and time as a DateTime object.
         * @return `DateTime` The current date and time.
         */
        DateTime now() {
            return RTC_DS3231::now();
        }

        /**
         * @brief Manually adjust the RTC time.
         * 
         * @param month The month (1-12).
         * @param day The day of the month (1-31)
         * @param year The full year (e.g., 2024)
         * @param hour The hour (0-23)
         * @param minute The minute (0-59)
         * @param second The second (0-59)
         */
        void manualAdjust(
            uint8_t month, uint8_t day, uint16_t year,
            uint8_t hour, uint8_t minute, uint8_t second
        ) {
            RTC_DS3231::adjust(DateTime(year, month, day, hour, minute, second));
        }

        /**
         * @brief Automatically adjust the RTC time to the compile date and time
         */
        void autoAdjust(void) {
            RTC_DS3231::adjust(DateTime(F(__DATE__), F(__TIME__)));
        }

        /**
         * @brief Get the current date components (month, day, year).
         * 
         * @tparam T Data type for the parameters (e.g., uint8_t, int).
         * @param month Pointer to store the current month.
         * @param day Pointer to store the current day.
         * @param year Pointer to store the current year.
         */
        template <typename T>
        void date(T *month = nullptr, T *day = nullptr, T *year = nullptr) {
            if (month != nullptr) *month = this->now().month();
            if (day != nullptr)   *day   = this->now().day();
            if (year != nullptr)  *year  = this->now().year();
        }

        /**
         * @brief Get the current time components (hour, minute, second).
         * 
         * @tparam T Data type for the parameters (e.g., uint8_t, int).
         * @param hour Pointer to store the current hour.
         * @param minute Pointer to store the current minute.
         * @param second Pointer to store the current second.
         */
        template <typename T>
        void time(
            String *am_pm = nullptr, 
            T *hour = nullptr, T *minute = nullptr, T *second = nullptr
        ) {
            if (!this->twelve_hour_format_flag) {
                if (hour != nullptr)  *hour  = this->now().hour();
            } else {
                if (am_pm != nullptr) *am_pm = (this->now().hour() < 12) ? "AM" : "PM";
                if (hour != nullptr)  *hour  = this->twelve_hour_format(); 
            }

            if (minute != nullptr)   *minute = this->now().minute();
            if (second != nullptr)   *second = this->now().second();
        }

        /**
         * @brief Get the current date as a formatter string (MM/DD/YYYY).
         * @return `String` The formatted date string
         */
        String datestr(void) {
            return String(this->now().month()) + "/"
                + String(this->now().day()) + "/"
                + String(this->now().year());
        }

        /**
         * @brief Get the current time as a formatted string (HH:MM:SS)
         * @return `String` The formatted time string
         */
        String timestr(void) {
            if (!this->twelve_hour_format_flag) {
                return String(this->now().hour()) + ":"
                    + String(this->now().minute()) + ":"
                    + String(this->now().second());
            }

            return String(this->twelve_hour_format()) + ":"
                + String(this->now().minute()) + ":"
                + String(this->now().second()) + " "
                + (this->now().hour() < 12 ? "AM" : "PM");
        }


        private:
            const char listDayOfWeek[7][12] = {
                "Sunday", "Monday", "Tuesday",
                "Wednesday", "Thursday", "Friday",
                "Saturday"
            };

            uint8_t twelve_hour_format() { // convert the hour component to 12-hours format
                return (this->now().hour() > 12) ? this->now().hour() - 12 : (this->now().hour() == 0) ? 12 : this->now().hour();
            }
};
