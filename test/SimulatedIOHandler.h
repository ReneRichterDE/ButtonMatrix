/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         SimulatedIOHandler.h
  -----------------------------------------------------------------------------
  @brief        IO simulation handler (required for unit testing)
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

#include <IOHandlerItf.h>
#include <Button.h>


/**
    @brief Provides an IO simulation impementation required for unit testing
    
*/
class SimulatedIOHandler : public RSys::IOHandlerItf
{
public:

    /** @brief see IOHandlerItf */
    virtual void pinMode(uint8_t pin, uint8_t mode);
    /** @brief see IOHandlerItf */
    virtual void digitalWrite(uint8_t pin, uint8_t val);
    /** @brief see IOHandlerItf */
    virtual int digitalRead(uint8_t pin);

    /**
        @brief  Simulate a button state
        @param  row
                Buttons row
        @param  col
                Buttons column
        @param  state
                New button state
    */
    void simButtonState(uint8_t row, uint8_t col, RSys::BTN_STATE state);

    /**
        @brief  Get the IO simulator instance (singleton)
        @param  rowPins
                Array of row pins
        @param  colPins
                Array of column pins
        @param  numRows
                Number of rows in the matrix
        @param  numCols
                Number of columns in the matrix
    */
    static inline SimulatedIOHandler& getInstance(
                                    uint8_t* rowPins, uint8_t* colPins,
                                    uint8_t numRows, uint8_t numCols)
    {            
        return *(new SimulatedIOHandler(rowPins, colPins, numRows, numCols));
    }

private:

    /**
        @brief  c'tor
        @param  rowPins
                Array of row pins
        @param  colPins
                Array of column pins
        @param  numRows
                Number of rows in the matrix
        @param  numCols
                Number of columns in the matrix
    */ 
    SimulatedIOHandler(
                    uint8_t* rowPins, uint8_t* colPins,
                    uint8_t numRows, uint8_t numCols);

    /**
        @brief  d'tor
    */ 
    virtual ~SimulatedIOHandler();

    /**
        @brief  Gets the first column that output pin is in LOW state
        @param  col
                Reference to the column number whose output pin is in LOW state
                (only valid if the method returns true)
        @return True if a column has been found, else false
                Array of column pins
    */ 
    bool getLowCol(uint8_t& col) const;

    /**
        @brief  Get the row number the pin is belonging to
        @param  pin
                Pin number to search for
        @param  row
                Reference to the row number the pin belongs to
                (only valid if the method returns true)
        @return True if the pin belongs to a row, else false
    */    
    bool getRowFromPin(uint8_t pin, uint8_t& row) const;

    /**
        @brief  Get the column number the pin is belonging to
        @param  pin
                Pin number to search for
        @param  col
                Reference to the column number the pin belongs to
                (only valid if the method returns true)
        @return True if the pin belongs to a column, else false
    */     
    bool getColFromPin(uint8_t pin, uint8_t& col) const;

    

    const uint8_t*  m_rowPins;      /** Array of row pins */
    const uint8_t*  m_colPins;      /** Array of column pins */
    const uint8_t   m_numRows;      /** Number of rows in the matrix */
    const uint8_t   m_numCols;      /** Number of columns in the matrix */

    int* m_pIoStates;                       /** Array of IO states (one for each column pin) */
    RSys::BTN_STATE* m_pButtonStates;   /** Array of button state (one for each button) */
};