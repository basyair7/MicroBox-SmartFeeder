/**
 *  @file FeederSys.cpp
 *  @version 1.2.0
 *  @author basyair7
 *  @date 2024
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "MicroBox/FeederSys.h"
#include "MicroBox/BlynkProgram.h"
#include "MicroBox/externprog.h"
#include "MicroBox/variable.h"

// Variables to track the timing for manual and automatic stepper control
unsigned long LastTimeRTC = 0;
bool stateStepperManual = false;
bool stateStepperAuto = false;

const uint16_t stepMotor = 800; // Number of steps for the motor

// Function to control the feeder manually
void FeederSys_manual() {
    if (stateStepperManual) {
        mystepper.step(stepMotor);
        delay(3000);
        mystepper.step(-stepMotor);
        delay(3000);
        stateStepperManual = false;
    }
}

// Function to control the feeder automatically based on the schedule
void FeederSys_auto() {
    for (const auto schedule : FeedingSchedule) {
        // Update RTC time every second
        if ((unsigned long)(millis() - LastTimeRTC) >= 1000) {
            LastTimeRTC = millis();

            // Check if the current time matches the feeding schedule
            if (rtcprog.timestr() == schedule) {
                stateStepperAuto = true;
            }
        }
    }

    // Run the stepper motor based on automatic control state
    if (stateStepperAuto) {
        mystepper.step(stepMotor);
        delay(3000);
        mystepper.step(-stepMotor);
        delay(3000);
        stateStepperAuto = false;
    }
}

// Main function to control the feeder system
void FeederSys_run() {
    // Read the control mode from EEPROM
    bool read_auto_control = myeeprom_prog.read(ADDR_EEPROM_AUTO_CONTROL);
    
    // Check water turbidity and control the relay accordingly
    if (WaterTurbidity.NTU_value >= 1.3 || (PHSensor.PH_value <= 6.5 || PHSensor.PH_value >= 7.2)) {
        digitalWrite(PIN_RELAY, ON);
    } else {
        digitalWrite(PIN_RELAY, OFF);
    }

    // Check the capacity of the ultrasonic sensor
    if (ultrasonic_capacity > 0 && WaterTurbidity.NTU_value <= 1.3) {
        // Run automatic feeder system if the control mode is set to auto
        if (read_auto_control) {
            FeederSys_auto();
        }
        // Run manual feeder system if the control mode is set to manual
        if (!read_auto_control && switch_state) {
            stateStepperManual = true;
            FeederSys_manual();
        }
    }
}
