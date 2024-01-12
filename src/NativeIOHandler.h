/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         NativeIOHandler.h
  -----------------------------------------------------------------------------
  @brief        Handles controller native IO for the ButtonMatrix
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

#ifndef NativeIOHandler_h
#define NativeIOHandler_h

#include <Arduino.h>
#include <IOHandlerItf.h>


namespace ButtonMatrix
{ 
    /**
        @brief Handles controller native digital IO ports   
        @implements IOHandlerItf
    */  
    class NativeIOHandler : public IOHandlerItf
    {
    public:

        virtual void pinMode(uint8_t pin, uint8_t mode)
        {
            ::pinMode(pin, mode);
        }

        virtual void digitalWrite(uint8_t pin, uint8_t val)
        {
            ::digitalWrite(pin, val);
        }

        virtual int digitalRead(uint8_t pin)
        {
            return ::digitalRead(pin);
        }

        /**
            @brief  Returns the default implementation for the native IO handler
            @return Reference to the implementation
        */ 
        static inline IOHandlerItf& getDefault()
        {
            static NativeIOHandler ioHandler;
            return ioHandler;
        }


    };

}


#endif // NativeIOHandler_h