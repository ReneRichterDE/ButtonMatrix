/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         Button.cpp
  -----------------------------------------------------------------------------
  @brief        Represents a button in the button matrix
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

#include "Button.h"


namespace RSys
{    

    // ------
    // Button
    // ------

    Button::Button(uint8_t number, bool bEnabled)
    //-----------------------------------------------------------------------------
    :   m_buttonNo(number),
        m_curState(STATE_RELEASED),
        m_prevState(STATE_UNINITIALIZED),
        m_lastAction(ACTION_NONE),
        m_bEnabled(bEnabled),
        m_stateChangeMillis(millis()),
        m_prevStateDuration(0),
        m_bSwallowNextRoseEvent(false),
        m_bStateChanged(false),
        m_bFell(false),
        m_bRose(false),
        m_bLongPress(false)
    {

    }



    uint8_t Button::getNumber() const
    //-----------------------------------------------------------------------------
    {
        return m_buttonNo;
    }



    bool Button::isEnabled() const
    //-----------------------------------------------------------------------------
    {
        return m_bEnabled;
    }



    void Button::setEnabled(bool bEnabled)
    //-----------------------------------------------------------------------------
    {
        m_bEnabled = bEnabled;
    }



    Button::STATE Button::getCurState() const
    //-----------------------------------------------------------------------------
    {
        return m_bEnabled ? m_curState : STATE_RELEASED;
    }



    Button::STATE Button::getPrevState() const
    //-----------------------------------------------------------------------------
    {
        return m_prevState;
    }


    bool Button::isPressed() const
    //-----------------------------------------------------------------------------
    {
        // Only report pressed if button is enabled
        return STATE_PRESSED == m_curState && m_bEnabled;
    }



    bool Button::isLongPressed(uint16_t ms) const
    //-----------------------------------------------------------------------------
    {
        bool longpress = !longpress && isPressed() && getCurStateDuration() >= ms;

        return longpress;
    }



    unsigned long Button::getCurStateDuration() const
    //-----------------------------------------------------------------------------
    {
        return millis() - m_stateChangeMillis;
    }



    unsigned long Button::getPrevStateDuration() const
    //-----------------------------------------------------------------------------
    {
        return m_prevStateDuration;
    }


    void Button::swallowNextRoseEvent(bool bSwallow)
    //-----------------------------------------------------------------------------
    {
        m_bSwallowNextRoseEvent = bSwallow;        
    }


    void Button::forceReleased()
    //-----------------------------------------------------------------------------
    {
        // update the state to RELEASED
        updateState(STATE_RELEASED);
        // reset any state change flags
        m_bRose = m_bFell = m_bStateChanged = false;
    }



    bool Button::updateState(const STATE newState)
    //-----------------------------------------------------------------------------
    {
        // we just update if the new state differs from the current one
        if (newState != m_curState)
        {
            m_bRose = m_bFell = false;
            m_prevStateDuration = getCurStateDuration();
            m_stateChangeMillis = millis();

            m_prevState = m_curState;
            m_curState = newState;

            // if button is disabled we do not report a state change
            m_bStateChanged = m_bEnabled;
            m_bFell = STATE_PRESSED == m_curState;
            if (STATE_RELEASED == m_curState)
            {
                // Reset any long press
                m_bLongPress = false;
                // just report rose when swallow is not set
                m_bRose = !m_bSwallowNextRoseEvent;
                // reset swallow so we can notify the next rose again
                m_bSwallowNextRoseEvent = false;
            }
        }

        return m_bStateChanged;
    }


    void Button::updateAction(const ACTION action)
    //-----------------------------------------------------------------------------
    {
        m_lastAction = action;
    }



    bool Button::hasStateChanged() const
    //-----------------------------------------------------------------------------
    {
        bool result = m_bStateChanged;
        m_bStateChanged = false;
        return result;
    }


    bool Button::fell() const
    //-----------------------------------------------------------------------------
    {
        // only report a fell when button is enabled
        bool result = m_bFell && m_bEnabled;
        m_bFell = false;
        m_bStateChanged = false;
        return result;
    }



    bool Button::rose() const
    //-----------------------------------------------------------------------------
    {
        // only report a rose when button is enabled
        bool result = m_bRose && m_bEnabled;
        m_bRose = false;
        m_bStateChanged = false;
        return result;
    }


    Button::ACTION Button::getLastAction(bool resetafter) const
    //-----------------------------------------------------------------------------
    {
        ACTION act = m_lastAction;
        if (resetafter)
        {
            m_lastAction = ACTION_NONE;
        }

        return act;
    }

}