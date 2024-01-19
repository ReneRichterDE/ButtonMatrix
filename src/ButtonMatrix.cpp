/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         ButtonMatrix.cpp
  -----------------------------------------------------------------------------
  @brief        Simple interface to a button matrix connected to an Arduino
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         29.12.2023
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

#include"ButtonMatrix.h"


namespace RSys
{    
    
    ButtonMatrix::ButtonMatrix(
                        Button* buttons,
                        uint8_t* rowPins, uint8_t* colPins,
                        uint8_t numRows, uint8_t numCols,
                        IOHandlerItf& ioItf)
    //-----------------------------------------------------------------------------
    :   m_pButtons(buttons),
        m_rowPins(rowPins),
        m_colPins(colPins),
        m_numRows(numRows),
        m_numCols(numCols),
        m_ioItf(ioItf),
        m_scanInterval(s_defaultScanInterval),
        m_lastScan(0),
        m_numButtons(numRows * numCols),
        m_buttonEventCallback(NULL)

    {

    }



    void ButtonMatrix::setScanInterval(uint16_t scanInterval)
    //-----------------------------------------------------------------------------
    {
        m_scanInterval = scanInterval;
    }


    
    bool ButtonMatrix::init()
    //-----------------------------------------------------------------------------
    { 
        // set all row pins as INPUT_PULLUP
        for (uint8_t row = 0; row < m_numRows; row++)
        {
            m_ioItf.pinMode(m_rowPins[row], INPUT_PULLUP);
        }

        // set all col pins to HIGH and then as INPUT
        // the update routine will set them later as
        // necessary
        for (uint8_t col = 0; col < m_numCols; col++)
        {
            m_ioItf.digitalWrite(m_colPins[col], HIGH);
            m_ioItf.pinMode(m_colPins[col], INPUT);
        }

        return true;
    }


    
    bool ButtonMatrix::update()
    //-----------------------------------------------------------------------------
    {
        bool hasAnyButtonChanged = false;

        // just scan if the minimum scan interval has elapsed
        if (millis() - m_lastScan >= m_scanInterval)
        {
            // iterate through all columns
            for (uint8_t col = 0; col < m_numCols; col++)
            {
                // set pin mode for the current column pin to OUTPUT
                m_ioItf.pinMode(m_colPins[col], OUTPUT);
                // pull down the output pin
                m_ioItf.digitalWrite(m_colPins[col], LOW);
                // iterate through all rows
                for (uint8_t row = 0; row < m_numRows; row++)
                {
                    Button* pBut = getButton(row, col);                    
                    bool bChanged = pBut->UpdateState(
                                            (m_ioItf.digitalRead(m_rowPins[row]) == LOW)
                                            ? Button::STATE_PRESSED 
                                            : Button::STATE_RELEASED);
                    // we need to report back if any button has changed its state
                    hasAnyButtonChanged = hasAnyButtonChanged || bChanged;
                }
                // set column pin to HIGH and INPUT again
                // necessary to allow detection of multiple buttons pressed in the
                // same row and not causing a short in this situation
                m_ioItf.digitalWrite(m_colPins[col], HIGH);
                m_ioItf.pinMode(m_colPins[col], INPUT);
            }  

            // lets remember our last scan timestamp
            m_lastScan = millis();
        }

        return hasAnyButtonChanged;
    }



    Button* ButtonMatrix::getButton(uint16_t idx) const
    //-----------------------------------------------------------------------------
    {       
        // make sure to only return a valid button inside the valid range
        return ((idx < m_numButtons) ? &m_pButtons[idx] : NULL);
    }


    uint16_t ButtonMatrix::getNumButtons() const
    //-----------------------------------------------------------------------------
    {
        return m_numButtons;
    }


    void ButtonMatrix::registerButtonEventCallback(buttonEventFnc cb)
    //-----------------------------------------------------------------------------
    {
        m_buttonEventCallback = cb;
    }



    Button* ButtonMatrix::getButton(uint8_t row, uint8_t col)
    //-----------------------------------------------------------------------------
    {
        Button* pButton = NULL;
        // make sure to only return a valid button inside the valid range
        if (m_numRows > row && m_numCols > col)
        {
            pButton = &m_pButtons[row * m_numCols + col];
        }

        return pButton;
    }

}