/** 
 *  @file __main__.c
 *  @version 1.2.0
 *  @brief Main program for the Arduino framework.
 *  @details This file contains the main entry of the Arduino program.
 *           Be careful when modifying this file, as it may affect the entire program's behavior.
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

#include "MicroBox/main.h"

/**
 * @brief Initializes the program by delegating to the Main class setup.
 * @param baudRate Baud rate for serial communication.
 * @param erase_eeprom Boolean flag to erase EEPROM data (default: false).
 * @details This function serves as the `main_setup` implementation from `main.h`. 
 *          It calls the `Main::setup` function for user-defined initialization logic.
 */
void setup() {
  // Initialize the main program with serial communication speed set to 115200 bps.
  // EEPROM program loading is disable (false)
  Main::SETUP(115200, false);
}


/**
 * @brief Executes the main program loop by delegating to the Main class loop.
 * @details This function serves as the `main_loop` implementation from `main.h`.
 */
void loop() {
  Main::LOOP();
}
