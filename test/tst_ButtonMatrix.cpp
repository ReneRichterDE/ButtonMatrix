
#include <Arduino.h>
#include <unity.h>

#include <ButtonMatrix.h>
#include "SimulatedIOHandler.h"


//static const uint32_t c_uiMonitorBaud = 115200; // USB monitoring baud rate

const uint16_t longPressDuration = 2000; /** Minimum duration of a long press */

const uint8_t COLS = 3; /** Number of button matrix columns */
const uint8_t ROWS = 3; /** Number of button matrix rows */

uint8_t colPins[COLS] = {4,5,6}; /** Button matrix column pins */
uint8_t rowPins[ROWS] = {0,1,2}; /** Button matrix row pins */


/** Button matrix button definitons */
RSys::Button buttons[ROWS][COLS] =
{
    { (1), (2), (3) },
    { (4), (5), (6) },
    { (7), (8), (9) }
};

/** IO simulator */
SimulatedIOHandler& simIO = SimulatedIOHandler::getInstance(rowPins, colPins, ROWS, COLS);

/** Button matrix */
RSys::ButtonMatrix matrix((RSys::Button*)buttons, rowPins, colPins, ROWS, COLS, simIO);


void setUp()
{
    
    
}

void tearDown()
{
    // clean stuff up here
    
}


void test_each_button_isolated()
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


void test_long_press()
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
    } else
    {
        TEST_ASSERT_MESSAGE(false, "Matrix not updated although it should!");
    }
}



void setup()
{
    matrix.init();
    matrix.setScanInterval(0);

    delay(2000); // service delay
    UNITY_BEGIN();

    RUN_TEST(test_each_button_isolated);
    RUN_TEST(test_long_press);

    UNITY_END(); // stop unit testing
}


void loop()
{
}