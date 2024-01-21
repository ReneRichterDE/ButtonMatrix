/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         ButtonMatrix.h
  -----------------------------------------------------------------------------
  @brief        Simple interface to a button matrix connected to an Arduino
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         29.12.2023
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

#ifndef ButtonMatrix_h
#define ButtonMatrix_h


#include <Arduino.h>

#include "Button.h"
#include "NativeIOHandler.h"



namespace RSys
{    
    /**
        @brief Provides a simple interface for using a button matrix with Arduino
               (similar to KeyMap but with more flexibility and a more object oriented approach)
               Used by the ButtonMatrix class, but can also be used standalone
      
    */    
    class ButtonMatrix
    {
    public:

        /**
            @brief  Button event callback type
            @param  Button&
                    Reference to the button
        */  
        typedef void (*btnEventFnc)(Button&);

        /**
            @brief  c'tor
            @param  buttons
                    Pointer to the two dimensional button object array of size numRows * numCols
            @param  rowPins
                    Pointer to the one dimensional array of row pins of size numRows
            @param  colPins
                    Pointer to the one dimensional array of column pins of size numCols
            @param  numRows
                    Number of rows the button matrix has
                    (the number of elements in the rowPins array must be adequate)
            @param  numCols
                    Number of columns the button matrix has
                    (the number of elements in the colPins array must be adequate)
            @param  ioItf
                    Reference to the IO handler implementation to be used                    
        */
        ButtonMatrix(
                Button* buttons, uint8_t* rowPins, uint8_t* colPins,
                uint8_t numRows, uint8_t numCols,
                IOHandlerItf& ioItf = NativeIOHandler::getDefault());

        /**
            @brief  Gets the current scan interval                    
            @return Scan interval in ms
        */
        inline uint16_t getScanInterval() const { return m_scanInterval; }

        /**
            @brief  Sets the interval in ms the button matrix state is queried 
                    This is used for debouncing as well as to limit CPU time usage
                    (default is 20 ms)
            @param  scanInterval
                    Minimum interval between to button matrix scan processes
        */
        void setScanInterval(uint16_t scanInterval);

        /**
            @brief  Initializes the button matrix
                    (make sure to call init() once in the Arduinos setup() function!)
            @return True if succeeded
        */
        bool init();

        /**
            @brief  Call update() to update the button matrix state. 
                    Should be called each time the Arduinos loop() function is executed
                    (the function itself takes care of complying with the the scanInterval)
            @return True if the state of any button in the matrix has changed during the scan
        */      
        bool update();

        /**
            @brief  Gets the button object with the given index into the matrix
            @param  idx
                    Index into the matrix (staring with 0 and limited by numRows * numCols - 1)
            @return Pointer to the button object at the given index
                    or NULL if the index is out of range
        */ 
        Button* getButton(uint16_t idx) const;

        /**
            @brief  Gets the button object at the given matrix position
            @param  row
                    Button row (0..numRows-1)
            @param  col
                    Button cokumn (0..numCols-1)
            @return Pointer to the button object at the given position
                    or NULL if the position is out of range
        */
        Button* getButton(uint8_t row, uint8_t col);        

        /**
            @brief  Gets the number of buttons in the matrix
            @return Number of buttons
        */ 
        inline uint16_t getNumButtons() const { return m_numButtons; }

        /**
            @brief  Gets the number of rows in the matrix
            @return Number of rows
        */ 
        inline uint8_t getNumRows() const { return m_numRows; }

        /**
            @brief  Gets the number of columns in the matrix
            @return Number of columns
        */
        inline uint8_t getNumCols() const { return m_numCols; }

        /**
            @brief  Gets minimum duration in ms after which a long press is detected
            @return Duration in ms
        */
        inline uint16_t getLongPressDuration() const { return m_LongPressMS; }

        /**
            @brief  Set the duration in ms after that a long press for a particular button is detected
            @param  ms
                    Duration in ms
        */
        void setMinLongPressDuration(uint16_t ms);

        /**
            @brief  Register a callback function to get notified when a button activity has been performed
                    Please note: Only one callback can be registered. Subsequent calls will overwrite functions
                    previously set!
            @param  cb
                    Callback function
        */
        void registerButtonActionCallback(btnEventFnc cb);

        /**
            @brief  Register a callback function to get notified when a buttons state has changed
                    Please note: Only one callback can be registered. Subsequent calls will overwrite functions
                    previously set!
            @param  cb
                    Callback function
        */        
        void registerButtonStateEventCallback(btnEventFnc cb);


    private:

        Button*         m_pButtons;     /** Pointer to button array */
        const uint8_t*  m_rowPins;      /** Array of row pins */
        const uint8_t*  m_colPins;      /** Array of column pins */
        const uint8_t   m_numRows;      /** Number of rows in the matrix */
        const uint8_t   m_numCols;      /** Number of columns in the matrix */
        IOHandlerItf&   m_ioItf;        /** IO handler interface to use for digital IO */

        uint16_t        m_scanInterval; /** Scan interval in ms */
        unsigned long   m_lastScan;     /** Timestamp (millis) of the last scan */

        uint16_t        m_LongPressMS;  /** Time in ms a after that a long press is determined */

        const uint16_t  m_numButtons;   /** Total number of button. Just to avoid recurring calculations */

        btnEventFnc     m_buttonActionCallback; /** Button action callback */
        btnEventFnc     m_buttonEventCallback;  /** Button state changed callback */

        static const uint16_t   s_defaultScanInterval = 20;     /** Default scan interval in ms */
        static const uint16_t   s_defaultLongPressMS = 2000;    /** Default interval for long press is 2000 ms */
    };
}


#endif // ButtonMatrix_h