#include "main.h"

#include "ThisRTOS.hpp"

// Includes MicroBox Headers
#include "MicroBox/BootButton.h"
#include "MicroBox/MyEEPROM.hpp"
#include "MicroBox/SysHandlers.h"
#include "MicroBox/ProgramWiFi.h"
#include "MicroBox/BlynkProgram.h"
#include "MicroBox/FeederSys.h"
#include "MicroBox/WebServer.h"
#include "MicroBox/info.h"

#include "MicroBox/LEDBoard.h"
#include "MicroBox/MyStepper.hpp"

#include "MicroBox/WaterTurbidity.hpp"
#include "MicroBox/PHSensor.hpp"
#include "MicroBox/Ultrasonic.hpp"
#include "MicroBox/DS3231rtc.hpp"

#include "MicroBox/externprog.h"
#include "MicroBox/variable.h"

#include "envWiFi.h"

// Initialize modules and global variables
MyEEPROM myeeprom_prog;                //!< EEPROM utility module
BootButton bootbtn(BOOTBUTTON, INPUT); //!< Boot button utility

PHSensorClass PHSensor;               //!< pH sensor module instance
WaterTurbidityClass WaterTurbidity;   //!< Water turbidity sensor module instance
Ultrasonic ultrasonic = Ultrasonic(PIN_TRIGGER, PIN_ECHO); //!< Ultrasonic sensor instance

DS3231rtc rtcprog; //!< RTC module instance
MyStepper mystepper = MyStepper(STEPS, PINOUT_STEPPER[0], PINOUT_STEPPER[1]); //!< Stepper motor instance

Info info;         //!< System information handler instance
RebootSys rebootsys; //!< System reboot handler instance

// Milliseconds trackers for task execution
unsigned long LastMillis = 0;     //!< Tracker for the last millisecond timestamp
unsigned long LastTimeReboot = 0; //!< Tracker for the last reboot time
bool RebootState = false;         //!< Tracks ESP reboot state

float ultrasonic_capacity; //!< Variable to store ultrasonic sensor capacity

/**
 * @brief Setup function for initializing hardware and modules.
 * @param baud Baud Rate for serial communication.
 * @details This function is executed once at startup. It initializes:
 *          - Serial communication
 *          - EEPROM and RTC Module
 *          - Sensors (pH Sensor, Water Turbidity, Ultrasonic)
 *          - WiFi, web server, and Blynk (if in STA Mode)
 */
void MicroBox_Main::setup(unsigned long baud) {
    Serial.begin(baud); //!< Initialize serial communication

    // Initialize EEPROM and RTC modules
    myeeprom_prog.initialize();
    rtcprog.begin(true);
    // rtcprog.manualAdjust(12, 19,2024, 11, 53, 50);

    // lfsprog.setupLFS(); //!< Available in Pro Version

    // Create FreeRTOS tasks
    ThisRTOS *prog = new ThisRTOS;
    xTaskCreateUniversal([](void *param) {
        static_cast<ThisRTOS*>(param)->vTask1(param);
    }, "Task1", 4096, NULL, 1, NULL, APP_CPU_NUM);
    xTaskCreateUniversal([](void *param) {
        static_cast<ThisRTOS*>(param)->vTask2(param);
    }, "Task2", 4096, NULL, 1, NULL, PRO_CPU_NUM);
    xTaskCreateUniversal([](void *param) {
        static_cast<ThisRTOS*>(param)->vTask3(param);
    }, "Task3", 4096, NULL, 1, NULL, PRO_CPU_NUM);
}

/**
 * @brief Task for running sensor updates and feeder control.
 * @param pvParameter Parameters for the task (not used).
 * @details This task continuously reads sensor data (water turbidity, pH, and ultrasonic sensor)
 *          and runs the feeder system based on sensor readings and predefined logic.
 *          It also prints the sensor values to the serial monitor for debugging and monitoring purposes.
 */
void ThisRTOS::vTask1(void *pvParameter) {
    (void) pvParameter; // Unused parameter
    
    // Initialize sensors and relay
    PHSensor.setup(PIN_PH_SENSOR);
    WaterTurbidity.setup(PIN_WATER_TURBIDITY);
    ultrasonic.begin();

    while(1) {
        // Run water turbidity sensor and update readings
        WaterTurbidity.run();

        // Run pH sensor and update readings
        PHSensor.run();
        
        // Get the distance reading from the ultrasonic sensor
        float distance = ultrasonic.getDistance();
        // Map the distance to a percentage representing capacity
        ultrasonic_capacity = map(distance, EMPTY + 1, FULL, 0, 100);
        // Ensure ultrasonic_capacity is within the valid range
        ultrasonic_capacity = (ultrasonic_capacity < 0) ? 0 : (ultrasonic_capacity > 100) ? 100 : ultrasonic_capacity;
        
        static unsigned long LastTimeMonitor = 0;
        if ((unsigned long) (millis() - LastTimeMonitor) >= 1000L) {
            LastTimeMonitor = millis();

            if (WiFi.getMode() == WIFI_AP || WiFi.status() == WL_CONNECTED) {
                Serial.println(F("**************************************\n"));
                Serial.print(F("Date: ")); Serial.println(rtcprog.datestr());
                Serial.print(F("Time: ")); Serial.println(rtcprog.timestr());
                Serial.println(F("**************************************\n"));
                Serial.print(F("WiFi Mode: "));
                Serial.println(WiFi.getMode() == WIFI_STA ? "WIFI_STA" : "WIFI_AP");
                
                // Print water turbidity sensor readings
                Serial.println(F("**************************************\n"));
                Serial.println(F("Water Turbidity: "));
                Serial.print(F("ADC Value: "));
                Serial.println(WaterTurbidity.ADC_value);
                Serial.print(F("NTU Value: "));
                Serial.println(WaterTurbidity.NTU_value, 2);
                
                // Print pH sensor readings
                Serial.println(F("**************************************\n"));
                Serial.println(F("PH Sensor: "));
                Serial.print(F("Voltage (V): "));
                Serial.println(PHSensor.voltage);
                Serial.print(F("PH Value: "));
                Serial.println(PHSensor.PH_value, 2);

                // Print ultrasonic sensor readings
                Serial.println(F("**************************************\n"));
                Serial.println(F("Ultrasonic: "));
                Serial.print(F("Get distance (cm): "));
                Serial.println(distance);
                Serial.print(F("Capacity: "));
                Serial.println(ultrasonic_capacity);
            }
        }

        // Delay the task for 50 milliseconds to control the task execution frequency
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @brief Task for running Blynk updates and web server OTA updates.
 * @param pvParameter Parameters for the task (not used).
 * @details This task continuously runs the Blynk update function and, if the 
 *          WiFi mode is set to Access Point (AP), it also updates the web server 
 *          for over-the-air (OTA) updates. This ensures that the Blynk application 
 *          remains responsive and the web server can handle OTA updates when needed.
 */
void ThisRTOS::vTask2(void *pvParameter) {
    (void) pvParameter; // Unused parameter

    // Read WiFi state and initialize
    bool x_state = myeeprom_prog.read(ADDR_EEPROM_WIFI_MODE);

    // Initialize WiFi program
    ProgramWiFi.setup(
        SSID_STA, PASSWORD_STA,
        SSID_AP, PASSWORD_AP
    );
    ProgramWiFi.initWiFi(x_state);


    // Initialize Blynk Program
    Blynk_setup();

    // Initialize WebServer Program
    WebServer.ServerInit();

    while(1) {
        // Run the Blynk update function to keep the Blynk application responsive
        Blynk_run();

        // If the WiFi mode is set to Access Point (AP), update the web server for OTA updates
        if (WiFi.getMode() == WIFI_AP) {
            WebServer.UpdateOTAloop();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


/**
 * @brief Task for updating system information and handling reboots.
 * @param pvParameter Parameters for the task (not used).
 * @details This task continuously updates the log for CPU and RAM usage, 
 *          handles system reboots based on certain conditions, and runs 
 *          auto state change logic. This ensures the system remains monitored 
 *          and can perform necessary reboots and state changes to maintain 
 *          optimal functionality.
 */
void ThisRTOS::vTask3(void *pvParameter) {
    (void) pvParameter; // Unused parameter

    // Initialize hardware components
    pinMode(PIN_RELAY, OUTPUT);
    LEDBoard::begin();
    bootbtn.begin();
    mystepper.setSpeed(SPEED_STEPPER);

    while(1) {
        // Update the log with current CPU and RAM usage
        if (WiFi.getMode() == WIFI_AP || WiFi.status() == WL_CONNECTED) info.updateLogCpuRamUsage();
        
        // Run the system reboot logic if necessary
        RebootSys::run(&LastTimeReboot, RebootState);
        
        // Execute automatic state change logic
        bootbtn.ChageWiFiMode();
        AutoChangeState::run();

        // Run the feeder system logic
        FeederSys_run();

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void MicroBox_Main::loop() {
    // The loop can contain other tasks or functionalities that need to be performed
}
