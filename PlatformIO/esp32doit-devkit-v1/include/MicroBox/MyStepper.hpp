#pragma once

#include <Arduino.h>
#include <Stepper.h>

class MyStepper : public Stepper {
    public:
        MyStepper(uint32_t number_of_steps, uint8_t motor_pin_1, uint8_t motor_pin_2)
            : Stepper(number_of_steps, motor_pin_1, motor_pin_2) 
        {
            this->number_of_steps = number_of_steps;
        }
        MyStepper(uint32_t number_of_steps, uint8_t motor_pin_1, uint8_t motor_pin_2,
                                            uint8_t motor_pin_3, uint8_t motor_pin_4)
            : Stepper(number_of_steps, motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4)
        {
            this->number_of_steps = number_of_steps;
        }
        MyStepper(uint32_t number_of_steps, uint8_t motor_pin_1, uint8_t motor_pin_2,
                                            uint8_t motor_pin_3, uint8_t motor_pin_4,
                                            uint8_t motor_pin_5)
            : Stepper(number_of_steps, motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4, motor_pin_5)
        {
            this->number_of_steps = number_of_steps;
        }

        void setSpeed(long whatSpeed) {
            Stepper::setSpeed(whatSpeed);
        }

        void step(uint32_t number_of_steps, bool degress_convert = false) {
            if (degress_convert) Stepper::step(this->stepsToDegress(number_of_steps));
            else Stepper::step(number_of_steps);
        }

    private:
        uint32_t number_of_steps;

        float stepsToDegress(int degress) {
            return ((float)this->number_of_steps * ((float)degress / 360.0));
        }
};
