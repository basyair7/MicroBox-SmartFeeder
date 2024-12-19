/**
 *  @file info.h
 *  @version 1.2.0
 *  @brief This file contains the Info class for logging system performance metrics.
 *  @date 2024
 *  @author 
 *  basyair7
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
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>

#define PROJECT_NAME     "MicroBOX (SmartFeeder Lite)"
#define PROJECT_CODE     "MicroBox"

#define PROJECT_VERSION  "2.0.0"
#define HARDWARE_VERSION "2.0.0"
#define SOFTWARE_VERSION "1.2.0"
#define BUILD_DATE       "12/16/2024"
#define REGION_NAME      "INDONESIA"

class Info_t {
    protected:
        const std::string projectName     = PROJECT_NAME;
        const std::string projectCode     = PROJECT_CODE;
        const std::string projectVersion  = PROJECT_VERSION;
        const std::string hardwareVersion = HARDWARE_VERSION;
        const std::string softwareVersion = SOFTWARE_VERSION;
        const std::string buildDate       = BUILD_DATE;
        const std::string regionName      = REGION_NAME;
};

class Info {
    public:
        void logInitialHeapMemoryState();
        void updateLogCpuRamUsage();

    private:
        void calculateCpuUsage();
};

extern float freeHeapMemory, totalHeapMemory;
