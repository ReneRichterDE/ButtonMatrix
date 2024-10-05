/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         MultiMCPHandler.h
  -----------------------------------------------------------------------------
  @brief        Handles multiple connected MCPs
  -----------------------------------------------------------------------------
  @author       Rene Richter
  @contact
  @date         05.10.2024
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

#ifndef MultiMcpHandler_h
#define MultiMcpHandler_h

#include <Arduino.h>
#include <IOHandlerItf.h>


namespace RSys
{
    template <class IOHandler, class MCPImpl>
    class MultiMCPHandler : public IOHandlerItf
    {
    public:

        virtual void pinMode(uint8_t vPin, uint8_t mode)
        {
            IOHandlerItf* pHandler = getHandler(vPin);
            if (NULL != pHandler)
            {
                pHandler->pinMode(getPhysicalPin(vPin), mode);
            }
        }

        virtual void digitalWrite(uint8_t vPin, uint8_t val)
        {
            IOHandlerItf* pHandler = getHandler(vPin);
            if (NULL != pHandler)
            {
                pHandler->digitalWrite(getPhysicalPin(vPin), val);
            }
        }

        virtual int digitalRead(uint8_t vPin)
        {
            int val = LOW;

            IOHandlerItf* pHandler = getHandler(vPin);
            if (NULL != pHandler)
            {
                val = pHandler->digitalRead(getPhysicalPin(vPin));
            }

            return val;
        }

        /**
            @brief  Returns the implementation for an MultiMCPHandler
            @param  mcpImpl
                    Array of MCP instances
            @param  numMCPs
                    Number of MCP instances in the array
            @return Reference to the handler interface
        */
        static inline IOHandlerItf& getInstance(MCPImpl* mcpImpl, const uint8_t numMCPs)
        {
            return *(new MultiMCPHandler(mcpImpl, numMCPs));
        }

        /**
            @brief d'tor
        */
        ~MultiMCPHandler()
        {
            for (uint8_t idx = 0; idx < m_numHandlers; idx++)
            {
                delete m_pHandlers[idx];
                m_pHandlers[idx] = NULL;
            }
            delete [] m_pHandlers;
            m_pHandlers = NULL;
        }


    private:

        /**
            @brief  c'tor
            @param  mcpImpl
                    Array of MCP instances
            @param  numMCPs
                    Number of MCP instances in the array
        */
        MultiMCPHandler(MCPImpl* mcpImpl, const uint8_t numMCPs)
        :    m_numHandlers(numMCPs)
        {
            m_pHandlers = new IOHandlerItf*[numMCPs];
            for (uint8_t idx = 0; idx < numMCPs; idx++)
            {
                m_pHandlers[idx] = &IOHandler::getInstance(mcpImpl[idx]);
            }
        }

        /**
            @brief  Returns the handler associated to the virtual pin
            @param  vPin
                    Virtual pin
            @return Pointer to the handler or NULL if out of range
        */
        IOHandlerItf* getHandler(uint8_t vPin)
        {
            const uint8_t idxHandler = vPin / s_HandlerIORange;
            IOHandlerItf* pHandler = (idxHandler < m_numHandlers) ? m_pHandlers[idxHandler] : NULL;
            return pHandler;
        }

        /**
            @brief  Returns the physical pin for the virtual pin given
            @param  vPin
                    Virtual pin
            @return Physical pin
        */
        inline const uint8_t getPhysicalPin(uint8_t vPin) const
        {
            return vPin % s_HandlerIORange;
        }


        IOHandlerItf** m_pHandlers;         /** IOHanlder interface array */
        const uint8_t m_numHandlers;    /** Number of handlers in the array */

        static const uint8_t s_HandlerIORange = 100; /** Virtual pin range per handler */
    };

}

#endif // MultiMcpHandler_h
