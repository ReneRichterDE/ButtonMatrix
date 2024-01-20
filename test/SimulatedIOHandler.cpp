
#include "SimulatedIOHandler.h"

#include <unity.h>


void SimulatedIOHandler::pinMode(uint8_t pin, uint8_t mode)
//-----------------------------------------------------------------------------
{
    
}


void SimulatedIOHandler::digitalWrite(uint8_t pin, uint8_t val)
//-----------------------------------------------------------------------------
{
    uint8_t col = 0;
    if (getColFromPin(pin, col))
    {
        m_pIoStates[col] = val;
    }
}


int SimulatedIOHandler::digitalRead(uint8_t pin)
//-----------------------------------------------------------------------------
{
    TEST_MESSAGE("digitalRead");
    int val = HIGH;
    uint8_t row = 0;

    if (getRowFromPin(pin, row))
    {        
        uint8_t col = 0;
        if (getLowCol(col))
        {
            TEST_MESSAGE("Low detected");
            val =  (RSys::Button::STATE_RELEASED == m_pButtonStates[row * m_numCols + col])
                    ? HIGH
                    : LOW;
        }
    }
    else
    {
        uint8_t col = 0;
        if (getColFromPin(pin, col))
        {
            val = m_pIoStates[col];
        }
    }

    return val;
}


void SimulatedIOHandler::simButtonState(
                                    uint8_t row, uint8_t col,
                                    RSys::Button::STATE state)
//-----------------------------------------------------------------------------
{
    m_pButtonStates[row * m_numCols + col] = state;
}


SimulatedIOHandler::SimulatedIOHandler(
                                uint8_t* rowPins, uint8_t* colPins,
                                uint8_t numRows, uint8_t numCols)
//-----------------------------------------------------------------------------
:   m_rowPins(rowPins),
    m_colPins(colPins),
    m_numRows(numRows),
    m_numCols(numCols),
    m_pIoStates(NULL),
    m_pButtonStates(NULL)
{    
    m_pIoStates = new int[numCols];
    m_pButtonStates = new RSys::Button::STATE[numRows*numCols];
    for (uint8_t idx = 0; idx < numRows*numCols; idx++)
    {
        m_pButtonStates[idx] = RSys::Button::STATE_RELEASED;
    }

    for (uint8_t idx = 0; idx < m_numCols; idx++)
    {
        m_pIoStates[idx] = HIGH;
    }
}



SimulatedIOHandler::~SimulatedIOHandler()
//-----------------------------------------------------------------------------
{
    delete [] m_pButtonStates;
    m_pButtonStates = NULL;

    delete [] m_pIoStates;
    m_pIoStates = NULL;
}


bool SimulatedIOHandler::getLowCol(uint8_t& col) const
//-----------------------------------------------------------------------------
{
    bool found = false;

    col = 0;
    do
    {
        found = LOW == m_pIoStates[col];
        if (!found) col++;
    } while (!found && col < m_numCols);

    TEST_ASSERT_TRUE_MESSAGE(found, "No LOW column found");

    return found;
}



bool SimulatedIOHandler::getRowFromPin(uint8_t pin, uint8_t& row) const
//-----------------------------------------------------------------------------
{
    bool found = false;

    row = 0;
    do
    {
        found = pin == m_rowPins[row];
        if (!found) row++;
    } while (!found && row < m_numRows);

    return found;
}


bool SimulatedIOHandler::getColFromPin(uint8_t pin, uint8_t& col) const
//-----------------------------------------------------------------------------
{
    bool found = false;

    col = 0;
    do
    {
        found = pin == m_colPins[col];
        if (!found) col++;
    } while (!found && col < m_numCols);

    return found;
}