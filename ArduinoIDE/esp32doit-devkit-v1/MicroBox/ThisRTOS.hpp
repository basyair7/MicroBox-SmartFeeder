
#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/task.h>

class ThisRTOS {
    public:
        void vTask1(void *pvParameter);
        void vTask2(void *pvParameter);
        void vTask3(void *pvParameter);
};
