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

#include <Arduino.h>
#include <unity.h>

#include <ButtonMatrix.h>
#include "SimulatedIOHandler.h"

using namespace RSys;


/** Forward declarations for event handlers */
void event_Button_State_changed(Button&);
void event_Button_Action(Button&);



const uint8_t COLS = 3; /** Number of button matrix columns */
const uint8_t ROWS = 3; /** Number of button matrix rows */

uint8_t colPins[COLS] = {4,5,6}; /** Button matrix column pins */
uint8_t rowPins[ROWS] = {0,1,2}; /** Button matrix row pins */


/** @brief Button matrix button definitons */
Button buttons[ROWS][COLS] =
{
    { (1), (2), (3) },
    { (4), (5), (6) },
    { (7), (8), (9) }
};

/** @brief IO simulator */
SimulatedIOHandler& simIO = SimulatedIOHandler::getInstance(rowPins, colPins, ROWS, COLS);

/** @brief Button matrix */
ButtonMatrix matrix((Button*)buttons, rowPins, colPins, ROWS, COLS, simIO);


/** Global button pointer for event testing */
Button* pButton = NULL;   



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

    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    if (matrix.update())
    {
        simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
        delay(200);
        if (matrix.update())
        {
            TEST_ASSERT_MESSAGE(false, "Matrix has updated although scan interval has not yet elapsed!");
        }
        else
        {
            delay(500);
            simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
            if (!matrix.update())
            {
                TEST_ASSERT_MESSAGE(false, "Matrix has not updated although scan interval has elapsed!");
            }

            simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
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
            simIO.simButtonState(row, col, BTN_STATE_PRESSED);
            bool changed = matrix.update();
            TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
            if (changed)
            {
                Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->fell(), "Button press not detected!");
                }
            }

            simIO.simButtonState(row, col, BTN_STATE_RELEASED);
            changed = matrix.update();
            TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
            if (changed)
            {
                Button* pBut = matrix.getButton(row, col);
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
            simIO.simButtonState(row, col, BTN_STATE_PRESSED);        
        }

        bool changed = matrix.update();
        TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
        if (changed)
        {
            for (uint8_t col = 0; col < COLS; col++)
            {     
                Button* pBut = matrix.getButton(row, col);
                TEST_ASSERT_NOT_NULL_MESSAGE(pBut, "Button pointer is NULL!");
                if (NULL != pBut)
                {
                    TEST_ASSERT_TRUE_MESSAGE(pBut->fell(), "Button press not detected!");
                }
            }
        }        

        for (uint8_t col = 0; col < COLS; col++)
        {           
            simIO.simButtonState(row, col, BTN_STATE_RELEASED);
        }

        changed = matrix.update();
        TEST_ASSERT_TRUE_MESSAGE(changed, "Matrix did not signal a change");
        if (changed)
        {
            for (uint8_t col = 0; col < COLS; col++)
            {               
                Button* pBut = matrix.getButton(row, col);
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
    matrix.setMinLongPressDuration(1000);
    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    if (matrix.update())
    {
        Button* pBut = matrix.getButton(0, 0);
        delay(200);
        TEST_ASSERT_FALSE_MESSAGE(pBut->isLongPressed(matrix.getLongPressDuration()), "Long press detected earlier than expected!");
        delay(810);
        TEST_ASSERT_TRUE_MESSAGE(pBut->isLongPressed(matrix.getLongPressDuration()), "Long press not detected!");
        
        simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
        matrix.update();
    }
    else
    {
        TEST_ASSERT_MESSAGE(false, "Matrix not updated although it should!");
    }
}


/** @brief Test if long press detection works properly */
void test_skipped_rose_after_button_long_press()
//-----------------------------------------------------------------------------
{
    matrix.setMinLongPressDuration(500);
    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    if (matrix.update())
    {
        Button* pBut = matrix.getButton(0, 0);
        delay(510);
        TEST_ASSERT_TRUE_MESSAGE(pBut->isLongPressed(matrix.getLongPressDuration()), "Long press not detected!");        
        
        simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
        matrix.update();
        TEST_ASSERT_FALSE_MESSAGE(pBut->rose(), "rose was notified after long press. Shouldn't be the case!");        
    }
    else
    {
        TEST_ASSERT_MESSAGE(false, "Matrix not updated although it should!");
    }
}


/** @brief Test if button state change events work properly */
void test_button_state_events()
//-----------------------------------------------------------------------------
{
    pButton = NULL;
    matrix.registerButtonActionCallback(NULL);
    matrix.registerButtonStateEventCallback(event_Button_State_changed);

    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    matrix.update();

    TEST_ASSERT_NOT_NULL_MESSAGE(
                pButton,
                "pButton is NULL, but should have been set by the state changed event handler!");

    if (NULL != pButton)
    {
        TEST_ASSERT_MESSAGE(
                    BTN_STATE_PRESSED == pButton->getCurState(),
                    "Button state is not PRESSED although it should!");
    }


    pButton = NULL;
    simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
    matrix.update();

    TEST_ASSERT_NOT_NULL_MESSAGE(
                pButton,
                "pButton is NULL, but should have been set by the state changed event handler!");   

    if (NULL != pButton)
    {
        TEST_ASSERT_MESSAGE(
                    BTN_STATE_RELEASED == pButton->getCurState(),
                    "Button state is not RELEASED although it should!");
    }                 

    matrix.registerButtonStateEventCallback(NULL);
    pButton = NULL;
}


/** @brief Test if button click action is detected and notified properly */
void test_button_action_event_click()
//-----------------------------------------------------------------------------
{
    pButton = NULL;
    matrix.registerButtonStateEventCallback(NULL);
    matrix.registerButtonActionCallback(event_Button_Action);

    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    matrix.update();

    TEST_ASSERT_NULL_MESSAGE(
                pButton,
                "pButton is not NULL, but should NOT have been set by the action event handler!");   

    simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
    matrix.update();

    TEST_ASSERT_NOT_NULL_MESSAGE(
                pButton,
                "pButton is NULL, but should have been set by the action event handler!");   

    if (NULL != pButton)
    {
        TEST_ASSERT_MESSAGE(
                    BTN_ACTION_CLICK == pButton->getLastAction(),
                    "Button click action not detected!");
    }

    matrix.registerButtonActionCallback(NULL);
    pButton = NULL;
}


/** @brief Test if button long press action is detected and notified properly */
void test_button_action_event_longpress()
//-----------------------------------------------------------------------------
{
    pButton = NULL;
    matrix.setMinLongPressDuration(600);
    matrix.registerButtonStateEventCallback(NULL);
    matrix.registerButtonActionCallback(event_Button_Action);

    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    matrix.update();
   
    TEST_ASSERT_NULL_MESSAGE(
                pButton,
                "pButton is not NULL, but should NOT have been set by the action event handler!");   

    delay(matrix.getLongPressDuration()+10);
    
    matrix.update();

    TEST_ASSERT_NOT_NULL_MESSAGE(
                pButton,
                "pButton is NULL, but should have been set by the action event handler!");   

    if (NULL != pButton)
    {
        TEST_ASSERT_MESSAGE(
                    BTN_ACTION_LONG_PRESS == pButton->getLastAction(),
                    "Button long press action not detected!");
    }

    simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
    matrix.update();
    matrix.registerButtonActionCallback(NULL);
    pButton = NULL;
}


/** @brief Test if button long press action is detected and notified properly */
void test_button_action_skipped_event_after_longpress()
//-----------------------------------------------------------------------------
{
    pButton = NULL;
    matrix.setMinLongPressDuration(500);
    matrix.registerButtonStateEventCallback(NULL);
    matrix.registerButtonActionCallback(event_Button_Action);

    simIO.simButtonState(0, 0, BTN_STATE_PRESSED);
    matrix.update();
   
    TEST_ASSERT_NULL_MESSAGE(
                pButton,
                "pButton is not NULL, but should NOT have been set by the action event handler!");   

    delay(matrix.getLongPressDuration()+10);
    
    matrix.update();

    TEST_ASSERT_NOT_NULL_MESSAGE(
                pButton,
                "pButton is NULL, but should have been set by the action event handler!");   

    if (NULL != pButton)
    {
        TEST_ASSERT_MESSAGE(
                    BTN_ACTION_LONG_PRESS == pButton->getLastAction(),
                    "Button long press action not detected!");
    }

    pButton = NULL;
    simIO.simButtonState(0, 0, BTN_STATE_RELEASED);
    matrix.update();
    TEST_ASSERT_NULL_MESSAGE(
                    pButton,
                    "After long press and releasing the button, there shouldn't be a notification for CLICK!");


    matrix.registerButtonActionCallback(NULL);
    pButton = NULL;
}


/** @brief Button state changed event handler */
void event_Button_State_changed(Button& button)
//-----------------------------------------------------------------------------
{
    pButton = &button;
}


/** @brief Button action event handler */
void event_Button_Action(Button& button)
//-----------------------------------------------------------------------------
{
    pButton = &button;
}



void setup()
//-----------------------------------------------------------------------------
{
    matrix.init();
    matrix.setScanInterval(0);

    delay(2000); // service delay
    UNITY_BEGIN();

    // Standard function tests
    RUN_TEST(test_scan_interval);
    RUN_TEST(test_each_button_isolated);
    RUN_TEST(test_parallel_button_press);
    RUN_TEST(test_button_long_press);
    RUN_TEST(test_skipped_rose_after_button_long_press);
    
    // Eventing tests
    RUN_TEST(test_button_state_events);
    RUN_TEST(test_button_action_event_click);
    RUN_TEST(test_button_action_event_longpress);
    RUN_TEST(test_button_action_skipped_event_after_longpress);

    UNITY_END(); // stop unit testing
}


void loop()
//-----------------------------------------------------------------------------
{
}