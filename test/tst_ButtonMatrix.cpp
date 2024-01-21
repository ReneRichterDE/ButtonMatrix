/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         tst_ButtonMatrix.cpp
  -----------------------------------------------------------------------------
  @brief        ButtonMatrix (platformio unity) unit tests
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         21.01.2024
  @modified     -

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

#include <Arduino.h>
#include <unity.h>

#include <ButtonMatrix.h>
#include "SimulatedIOHandler.h"


const uint8_t COLS = 3; /** Number of button matrix columns */
const uint8_t ROWS = 3; /** Number of button matrix rows */

uint8_t colPins[COLS] = {4,5,6}; /** Button matrix column pins */
uint8_t rowPins[ROWS] = {0,1,2}; /** Button matrix row pins */


/** @brief Button matrix button definitons */
RSys::Button buttons[ROWS][COLS] =
{
    { (1), (2), (3) },
    { (4), (5), (6) },
    { (7), (8), (9) }
};

/** @brief IO simulator */
SimulatedIOHandler& simIO = SimulatedIOHandler::getInstance(rowPins, colPins, ROWS, COLS);

/** @brief Button matrix */
RSys::ButtonMatrix matrix((RSys::Button*)buttons, rowPins, colPins, ROWS, COLS, simIO);

/** @brief Runs before each test */
void setUp()
//-----------------------------------------------------------------------------
{
    
    
}

/** @brief Runs after each test */
void tearDown()
//-----------------------------------------------------------------------------
{
    // clean stuff up here
    
}


/** @brief Test if scan interval is properly respected */
void test_scan_interval()
//-----------------------------------------------------------------------------
{
    uint16_t scanIntervalSav = matrix.getScanInterval();
    matrix.setScanInterval(500);

    simIO.simButtonState(0, 0, RSys::Button::STATE_PRESSED);
    if (matrix.update())
    {
        simIO.simButtonState(0, 0, RSys::Button::STATE_RELEASED);
        delay(200);
        if (matrix.update())
        {
            TEST_ASSERT_MESSAGE(false, "Matrix has updated although scan interval has not yet elapsed!");
        }
        else
        {
            delay(500);
            simIO.simButtonState(0, 0, RSys::Button::STATE_PRESSED);
            if (!matrix.update())
            {
                TEST_ASSERT_MESSAGE(false, "Matrix has not updated although scan interval has elapsed!");
            }

            simIO.simButtonState(0, 0, RSys::Button::STATE_RELEASED);
            delay(600);
            matrix.update();
        }              
    }
    else
    {
        TEST_ASSERT_MESSAGE(false, "Matrix not updated although it should!");
    }



    matrix.setScanInterval(scanIntervalSav);
}


/** @brief Test each button in the matrix isolated (only one button at a time) */
void test_each_button_isolated()
//-----------------------------------------------------------------------------
{
    for (uint8_t row = 0; row < ROWS; row++)
    {
        for (uint8_t col = 0; col < COLS; col++)
        {
            simIO.simButtonState(row, col, RSys::Button::STATE_PRESSED);
            bool changed = matrix.update();
            TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
            if (changed)
            {
                RSys::Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->fell(), "Button press not detected!");
                }
            }

            simIO.simButtonState(row, col, RSys::Button::STATE_RELEASED);
            changed = matrix.update();
            TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
            if (changed)
            {
                RSys::Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->rose(), "Button released not detected!");
                }
            }
        }
    }
}


/** @brief Test if multiple buttons pressed at the same time work properly 
 *         columns based as this is the critical scenario)
 */
void test_parallel_button_press()
//-----------------------------------------------------------------------------
{    
    for (uint8_t row = 0; row < ROWS; row++)
    {
        for (uint8_t col = 0; col < COLS; col++)
        {
            simIO.simButtonState(row, col, RSys::Button::STATE_PRESSED);        
        }

        bool changed = matrix.update();
        TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
        if (changed)
        {
            for (uint8_t col = 0; col < COLS; col++)
            {     
                RSys::Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->fell(), "Button press not detected!");
                }
            }
        }        

        for (uint8_t col = 0; col < COLS; col++)
        {           
            simIO.simButtonState(row, col, RSys::Button::STATE_RELEASED);
        }

        changed = matrix.update();
        TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
        if (changed)
        {
            for (uint8_t col = 0; col < COLS; col++)
            {               
                RSys::Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->rose(), "Button released not detected!");
                }
            }
        }
    }
}


/** @brief Test if long press detection works properly */
void test_button_long_press()
//-----------------------------------------------------------------------------
{
    simIO.simButtonState(0, 0, RSys::Button::STATE_PRESSED);
    if (matrix.update())
    {
        RSys::Button* pBut = matrix.getButton(0, 0);
        delay(200);
        TEST_ASSERT_FALSE_MESSAGE(pBut->isLongPressed(1000), "Long press deteced earlier than expected!");
        delay(810);
        TEST_ASSERT_TRUE_MESSAGE(pBut->isLongPressed(1000), "Long press not deteced!");
        
        simIO.simButtonState(0, 0, RSys::Button::STATE_RELEASED);
        matrix.update();
    }
    else
    {
        TEST_ASSERT_MESSAGE(false, "Matrix not updated although it should!");
    }
}



void setup()
//-----------------------------------------------------------------------------
{
    matrix.init();
    matrix.setScanInterval(0);

    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_scan_interval);
    RUN_TEST(test_each_button_isolated);
    RUN_TEST(test_parallel_button_press);
    RUN_TEST(test_button_long_press);

    UNITY_END(); // stop unit testing
}


void loop()
//-----------------------------------------------------------------------------
{
}