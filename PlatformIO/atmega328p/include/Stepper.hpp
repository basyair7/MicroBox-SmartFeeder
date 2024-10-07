/*! @file Stepper.hpp
 *  @version 1.0.0
*/

#pragma once

#include "Arduino.h"
#include "Stepper.h"

class MyStepper {
public:
	MyStepper(uint32_t number_of_steps, uint8_t motor_pin_1,
			uint8_t motor_pin_2, uint8_t motor_pin_3, uint8_t motor_pin_4) : 
	mystepper(number_of_steps, motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4)
	{
		this->_number_of_steps = number_of_steps;
	}
	
	void setSpeed(uint32_t whatSpeed) {
		mystepper.setSpeed(whatSpeed);
	}
	
	void step(int degress) {
		mystepper.step(this->stepsToDegress(degress));
	}
	
private:
	Stepper mystepper;
	uint32_t _number_of_steps;
	
	float stepsToDegress(int degress) {
		return ((float)this->_number_of_steps * ((float)degress / 360.0));
	}
};