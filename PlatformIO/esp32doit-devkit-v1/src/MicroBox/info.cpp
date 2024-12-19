/**
 *  @file info.cpp
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

#include "MicroBox/info.h"

// Constants for periodic logging
unsigned long lastHeapMemoryLogTime = 0;         // Timestamp for the last memory log
const uint32_t heapMemoryLogInterval = 10000UL;  // Interval (in ms) between memory log outputs

// Variables for memory usage monitoring
float freeHeapMemory  = 0.0;
float totalHeapMemory = ESP.getFreeHeap() / 1024.0; // Stores total heap memory at startup (in KB)

// Variables for CPU usage monitoring
unsigned long lastCpuUsageCheckTime = 0;     // Timestamp for the last CPU usage calculation
float cpuUsageCore0 = 0;                    // CPU usage percentage for Core 0
float cpuUsageCore1 = 0;                    // CPU usage percentage for Core 1
unsigned long idleTimeCore0 = 0;            // Idle time for Core 0
unsigned long idleTimeCore1 = 0;            // Idle time for Core 1
unsigned long totalIdleTimeCore0 = 0;       // Total idle time for Core 0
unsigned long totalIdleTimeCore1 = 0;       // Total idle time for Core 1

/**
 * @brief Calculates the CPU usage based on idle task run time.
 * @details This function measures the time spent by the idle task and calculates CPU usage as a percentage.
 */
void Info::calculateCpuUsage() {
    unsigned long currentTime = millis(); // Current timestamp in milliseconds
    const uint32_t interval = 1000UL; // 1 second

    if (currentTime - lastCpuUsageCheckTime >= interval) {   // Check if 1 second has passed
        // Get the idle task run time for each core
        totalIdleTimeCore0 = esp_timer_get_time() - idleTimeCore0;
        totalIdleTimeCore1 = esp_timer_get_time() - idleTimeCore1;

        // Calculate CPU usage percentage for each core
        cpuUsageCore0 = ((float)(interval - totalIdleTimeCore0) / interval) * 100.0;
        cpuUsageCore1 = ((float)(interval - totalIdleTimeCore1) / interval) * 100.0;

        // Update the idle time for each core
        idleTimeCore0 = esp_timer_get_time();
        idleTimeCore1 = esp_timer_get_time();

        lastCpuUsageCheckTime = currentTime; // Update the timestamp
    }
}

void Info::logInitialHeapMemoryState() {
    // Log the initial heap memory state
    Serial.println();
    Serial.print(F("Total Heap\t: "));
    Serial.print(totalHeapMemory, 2);
    Serial.println(F(" KB"));
    Serial.print(F("Free Heap\t: "));
    Serial.print(ESP.getFreeHeap() / 1024.0, 2); // Convert bytes to KB
    Serial.println(F(" KB"));
}

void Info::updateLogCpuRamUsage() {
    this->calculateCpuUsage(); // Update CPU usage

    // Periodically log system performance metrics
    if (millis() - lastHeapMemoryLogTime >= heapMemoryLogInterval) {
        lastHeapMemoryLogTime = millis(); // Update the timestamp

        // Update memory metrics
        freeHeapMemory = ESP.getFreeHeap() / 1024.0; // Convert bytes to KB
        
        // Log CPU usage and memory statistics
        Serial.println();
        Serial.println(F("**** CPU Usage ****"));
        Serial.print(F("CPU Speed Core 0\t: "));
        Serial.print(ESP.getCpuFreqMHz());
        Serial.println(F(" MHz"));
        Serial.print(F("CPU Usage Core 0\t: "));
        Serial.print(cpuUsageCore0, 2);  // Print CPU usage percentage for Core 0
        Serial.println(F(" %"));
        Serial.print(F("CPU Speed Core 1\t: "));
        Serial.print(ESP.getCpuFreqMHz());
        Serial.println(F(" MHz"));
        Serial.print(F("CPU Usage Core 1\t: "));
        Serial.print(cpuUsageCore1, 2);  // Print CPU usage percentage for Core 1
        Serial.println(F(" %"));
        Serial.println();
        Serial.println(F("**** Memory Usage ****"));
        Serial.print(F("Total Heap\t: "));
        Serial.print(totalHeapMemory);
        Serial.println(F(" KB"));
        Serial.print(F("Free Heap\t: "));
        Serial.print(freeHeapMemory, 2);  // Print free heap memory
        Serial.println(F(" KB"));
        Serial.print(F("Used Heap\t: "));
        Serial.print(totalHeapMemory - freeHeapMemory, 2);  // Calculate used heap memory
        Serial.println(F(" KB"));
        Serial.println("\n");
    }
}
