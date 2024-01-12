/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         IOHandler.h
  -----------------------------------------------------------------------------
  @brief        Interface for controlling IO
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @date         11.01.2024
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

#ifndef IOHandlerItf_h
#define IOHandlerItf_h

#include <Arduino.h>


namespace ButtonMatrix
{ 
    /**
        @brief Abstract interface to handle IO      
    */  
    class IOHandlerItf
    {
    public:
    
        /**
            @brief  Sets the mode of a pin
            @param  pin
                    Pin number
            @param  mode
                    Mode to set
        */     
        virtual void pinMode(uint8_t pin, uint8_t mode) = 0;

        /**
            @brief  Sets the output pin to a particular state
            @param  pin
                    Pin number
            @param  val
                    State value
        */ 
        virtual void digitalWrite(uint8_t pin, uint8_t val) = 0;

        /**
            @brief  Reads the state of a pin
            @param  pin
                    Pin number
            @return Pin state
        */ 
        virtual int digitalRead(uint8_t pin) = 0;
    };



}


#endif // IOHandlerItf_h