/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         Example01_pressed.ino
  -----------------------------------------------------------------------------
  @brief        Example showing absolute basic button matrix usage of the
                isPressed() method.
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         21.01.2024
  @modified     -
  @copyright    (c) 2023-2024 Rene Richter
  @license      This library is free software; you can redistribute it and/or
                modify it under the terms of the GNU Lesser General Public
                License as published by the Free Software Foundation; version
                2.1 of the License.

                This library is distributed in the hope that it will be useful,
                but WITHOUT ANY WARRANTY; without even the implied warranty of
                MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
                See the GNU Lesser General Public License for more details.
  *****************************************************************************
*/

/**
 * What you need to do to work with ButtonMatrix:
 * 
 * 1. Add the library to your project
 * 2. Include the header file in main.ino/main.cpp (or wherever you need it)
 * 3. Either a "using namespace RSys;" or just prefix all ButtonMatrix types with "RSys::" (i.e. "RSys::Button")
 * 4. Define the column pins
 * 5. Define the row pins
 * 6. Define your buttons
 * 7. Create an instance of the ButtonMatrix passing the information of steps 4. to 6.
 * 8. Make sure to call the init() method in setup()
 * 9. Place a call to the update() method in loop() always before dealing with the state of the buttons
 */


#include <Arduino.h>
#include "ButtonMatrix.h" /** Include this header in order to work with the button matrix */



/** Everything in the ButtonMatrix library is within this namespace */
using namespace RSys;


static const uint32_t c_uiMonitorBaud = 115200; // USB monitoring baud rate

// -------------
// Button matrix
// -------------


const uint8_t COLS = 3; /** Number of button matrix columns */
const uint8_t ROWS = 3; /** Number of button matrix rows */

uint8_t colPins[COLS] = {7,8,9}; /** Button matrix column pins */
uint8_t rowPins[ROWS] = {4,5,6}; /** Button matrix row pins */

/** Button matrix button definitons */
RSys::Button buttons[ROWS][COLS] = {
    { (1), (2), (3) },
    { (4), (5), (6) },
    { (7), (8), (9) }
};

ButtonMatrix matrix((Button*)buttons, rowPins, colPins, ROWS, COLS);


void setup()
{
    Serial.begin(c_uiMonitorBaud);
    
    matrix.init();  /** Initialize the ButtonMatrix*/
}


void loop()
{
    Button* pButton = NULL;

    // Get the number of buttons in the matrix
    const uint16_t numButtons = matrix.getNumButtons();

    // Here we are just querying

    // Make sure to update the matrix frequently. There is a scan interval that defaults to 20ms but can be adjusted by matrix.setScanInterval(..).
    // The update doesn't do anything if the scan interval has not yes elapsed (debouncing and mc load reduction)
    if (matrix.update())
    {  
        // There was a change in any of the buttons  

        // Scan all buttons, except the last one
        for (uint16_t idx = 0; idx < numButtons; idx++)
        {        
            pButton = matrix.getButton(idx);    
            if (pButton->isPressed())
            {
                // Button is pressed
               Serial.print("Button pressed: ");Serial.println(pButton->getNumber());

               // For a more sophisticated example see Example02_fell_rose_lp example!
            }             
        }
    }

}
