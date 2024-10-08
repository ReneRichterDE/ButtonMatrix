/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         Example05_multi_mcp.ino
  -----------------------------------------------------------------------------
  @brief        Example showing how to use a button matrix connected to
                multiple MCPs
                Make sure to install the Adafruit MCP23017 Arduino Library from
                https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         05.10.2024
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
 * 3. Either add "using namespace RSys;" or just prefix all ButtonMatrix types with "RSys::" (i.e. "RSys::Button")
 * 4. Define the column pins
 * 5. Define the row pins
 * 6. Define your buttons
 * 7. Create an instance of the ButtonMatrix passing the information of steps 4. to 6.
 * 8. Make sure to call the init() method in setup()
 * 9. Place a call to the update() method in loop() always before dealing with the state of the buttons
 */


// *************************************************************************************************
//
// NOTE: In order to run this example you will have to install the Adafruit MCP23017 Arduino Library
// from https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library  !!!
//
// *************************************************************************************************


#include <Arduino.h>

// ButtonMatrix includes
#include "ButtonMatrix.h"         /** Include this header in order to work with the button matrix */
#include "AdafruitI2CIOHandler.h" /** This is required for the ButtonMatrix to work with the Adafruit i2c ioexpansion */
#include "MultiMCPHandler.h"      /** Include this for the multi MCP handler */

// Adafruit include
#include <Adafruit_MCP23X17.h>

const uint8_t numMCPs = 2;
Adafruit_MCP23X17 mcp[numMCPs]; /** Instances of the MCP23x17 controller interface */
const int mcp_addr[numMCPs] = {0x27, 0x28 }; /** Each MCP needs its own address */




/** Everything in the ButtonMatrix library is within this namespace */
using namespace RSys;


static const uint32_t c_uiMonitorBaud = 115200; // USB monitoring baud rate

// -------------
// Button matrix
// -------------


const uint16_t longPressDuration = 1000; /** Minimum duration of a long press */

const uint8_t COLS = 3; /** Number of button matrix columns */
const uint8_t ROWS = 3; /** Number of button matrix rows */

// Note: Each MCP board has its own number range
//       Board 1 goes from 0-99, board 2 goes from 100-199 and so on
//       The number within each range selects the physical pin on the board associated to the range
//       You can arrange pins across boards as you like
//       Pin number mapping for each board:
//          0 ..  7: GPA0 .. GPA7
//          8 .. 15: GPB0 .. GPB7
uint8_t colPins[COLS] = {100,101,102}; /** Button matrix column pins (0-2 on second mcp) */
uint8_t rowPins[ROWS] = {000,001,002}; /** Button matrix row pins    (0-2 on first mcp) */


/** Button matrix button definitons */
Button buttons[ROWS][COLS] = {
    { (1), (2), (3) },
    { (4), (5), (6) },
    { (7), (8), (9) }
};

// Note that we have to tell the ButtonMatrix to use the i2c io handler now (last c'tor param)
ButtonMatrix matrix(
                (Button*)buttons, rowPins, colPins, ROWS, COLS,
                MultiMCPHandler<AdafruitI2CIOHandler<Adafruit_MCP23X17>, Adafruit_MCP23X17>::getInstance(mcp, numMCPs));


void setup()
{
    Serial.begin(c_uiMonitorBaud);

    // Initialize all MCPs
    for (uint8_t idx = 0; idx < numMCPs; idx++)
    {
        if (!mcp[idx].begin_I2C(mcp_addr[idx])) // Adafruit_MCP23X17 specific initialization
        {
            Serial.println("Error.");
            while (1);
        }
    }

    matrix.init();  /** Initialize the ButtonMatrix*/
    //matrix.setInvertInput(); /** Uncomment if you get a pressed signal while button is released and vice versa */
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
            if (pButton->fell())
            {
                // Button has rose. The fell() method automatically resets the event so you cannot call it twice for the same occurrence of the event!
               Serial.print("Button fell ");Serial.println(pButton->getNumber());

            }
            else if (pButton->rose())
            {
                // Button has rose. The rose() method automatically resets the event so you cannot call it twice for the same occurrence of the event!
                Serial.print("Button rose ");Serial.println(pButton->getNumber());
            }
        }
    }

    // Now we check for the long press.
    // Note that this also when no change is detected by matrix.update() !
    for (uint16_t idx = 0; idx < numButtons; idx++)
    {
        // get the button with the defined index (you can also get buttons by row and column -> getButton(row, col))
        pButton = matrix.getButton(idx);
        if (pButton->isLongPressed(longPressDuration))
        {
            // Button is pressed long
            Serial.print("Button long pressed ");Serial.println(pButton->getNumber());

            // Note: After long press detection, you won't get a rose event by intention when button is being released!
        }
    }

}
