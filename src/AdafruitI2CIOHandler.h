/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         Adafruit_I2CIOHandler.h
  -----------------------------------------------------------------------------
  @brief        Handles IO connected via Adafruit I2C interface for the ButtonMatrix
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @contact      
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

#ifndef Adafruit_I2CIOHandler_h
#define Adafruit_I2CIOHandler_h

#include <Arduino.h>
#include <IOHandlerItf.h>


namespace RSys
{ 
    /**
        @brief  Helper to create an io handler instance
        @param  mcp
                Concrete I2C implementation object earlier instantiated        
    */
    #define ADFI2C(mcp) AdafruitI2CIOHandler<decltype(mcp)>::getInstance(mcp)



    /**
        @brief  Handles I2C IO by means of the Adafruit MCP23017 Arduino library implementation
        @tparam I2CImpl
                I2C handler implementation
        @implements IOHandlerItf      
    */
    template <class I2CImpl>
    class AdafruitI2CIOHandler : public IOHandlerItf
    {
    public:

        virtual void pinMode(uint8_t pin, uint8_t mode)
        {
            m_i2cImpl.pinMode(pin, mode);
        }

        virtual void digitalWrite(uint8_t pin, uint8_t val)
        {
            m_i2cImpl.digitalWrite(pin, val);
        }

        virtual int digitalRead(uint8_t pin)
        {
            return m_i2cImpl.digitalRead(pin);
        }


        static inline IOHandlerItf& getInstance(I2CImpl& i2cImpl)
        {            
            return *(new AdafruitI2CIOHandler(i2cImpl));
        }

    private:

        /**
            @brief  Returns the implementation for an Adafruit I2C handler
            @return Reference to the implementation
        */ 
        AdafruitI2CIOHandler(I2CImpl& i2cImpl)
        :   m_i2cImpl(i2cImpl)
        {        
        }

        I2CImpl& m_i2cImpl; /** Reference to the Adafruit I2C implementation */

    };

}


#endif // Adafruit_I2CIOHandler_h