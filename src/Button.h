/**
  *****************************************************************************
  Module        ButtonMatrix
  @file         Button.h
  -----------------------------------------------------------------------------
  @brief        Represents a button in the button matrix
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

#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include "ButtonBaseItf.h"


namespace RSys
{
    /**
        @brief Representation of a button.
               Used by the ButtonMatrix class, but can also be used standalone

    */
    class Button : public ButtonBaseItf
    {
    public:


        /**
            @brief  c'tor
            @param  number
                    The buttons number
            @param  bEnabled
                    False, if the button shall be disabled by default
                    A disabled button does not notify anything and always reports RELEASED state!
        */
        Button(uint8_t number, bool bEnabled = true);

        /**
            @brief  d'tor
        */
        virtual ~Button() {};

        /**
            @brief  Gets the buttons number
            @return The buttons number
        */
        uint8_t getNumber() const;

        /**
            @brief  Determines whether or not the button is enabled
            @return True, if the button is enabled
        */
        bool isEnabled() const;

        /**
            @brief  (Re)sets the buttons enabled state
            @param  bEnabled
                    True for enabled
                    A disabled button does not notify anything and always reports RELEASED state!
        */
        void setEnabled(bool bEnabled);

        /**
            @brief  Determines the current state of the button
            @return The buttons current state
        */
        BTN_STATE getCurState() const;

        /**
            @brief  Determines the previous state of the button
            @return The buttons previous state
        */
        BTN_STATE getPrevState() const;

        /**
            @brief  Determines whether or not the button is pressed
            @return True, if the button is currently pressed
        */
        bool isPressed() const;

        /**
            @brief  Determines whether or not the button is pressed longer
            @param  ms
                    Time period in ms after which a press duration is seen as a long press
            @return True, if the button is long pressed
        */
        bool isLongPressed(uint16_t ms) const;

        /**
            @brief  Determines the duration the button is in the current state
            @return The duration in ms (roll over after ~50 days!)
        */
        unsigned long getCurStateDuration() const;

        /**
            @brief  Determines the duration the button was in the previous state
            @return The duration in ms (roll over after ~50 days!)
        */
        unsigned long getPrevStateDuration() const;

        /**
            @brief  Indicates to the button that the next rose transition shall (or shall not)
                    be notified as a state change
                    (if you don't call this method is will always be notified)
            @param  bSwallow
                    If set to true the rose transition will not be notified.
                    You only need to set this to false if you have previously decided to swallow
                    and now want to revert that decision
        */
        void swallowNextRoseEvent(bool bSwallow = true);


        /**
            @brief  Determines whether or not the buttons state has changed
                    If the state had changed, the change flag will be reset internally!
            @return True, if the state has changed
        */
        bool hasStateChanged() const;

        /**
            @brief  Determines whether the buttons state fell (from RELEASED to PRESSED)
                    The fell flag is reset afterwards
            @return True, if the button state fell
        */
        bool fell() const;

        /**
            @brief  Determines whether the buttons state rose (from PRESSED to RELEASED)
                    The rose flag is reset afterwards
            @return True, if the button state rose
        */
        bool rose() const;

        /**
            @brief  Gets the last action executed on the button
            @param  resetafter
                    If true, the last action is reset to none afterwards
            @return Action last executed
        */
        BTN_ACTION getLastAction(bool resetafter = true) const;

    protected:

        /**
            @brief  Updates the button with a new state.
                    If the state is different to the current state, the change will be notified!
                    (Left the method public to allow usage independent of the ButtonMatrix)
            @return True, if the state has changed or false if the new state is the same as the previous
        */
        virtual bool updateState(const BTN_STATE newState);

        /**
            @brief  Updates the buttons last executed action
            @param  action
                    Action executed
        */
        virtual void updateAction(const BTN_ACTION action);

        /**
            @brief Determine if the click action shall be notified
            @return True, if click action shall be notified
        */
        virtual bool doNotifyClick();

    private:

        uint8_t m_buttonNo;     /** The buttons number */
        BTN_STATE   m_curState;     /** The buttons current state */
        BTN_STATE   m_prevState;    /** The buttons previous state */
        mutable BTN_ACTION  m_lastAction;      /** The last action executed on the button */

        bool m_bEnabled;                   /** Button is or isn't enabled */

        unsigned long m_stateChangeMillis; /** Time a which the buttons state changed last */
        unsigned long m_prevStateDuration; /** The duration the button was in its previous state */

        bool m_swallowNextRoseEvent;   /** Determines whether or not the next state change shall be swallowed and not be notified */

        mutable bool m_stateChanged;   /** Determines if the state has changed since the last state query */
        mutable bool m_fell;           /** Determines if the state fell recently (from RELEASED to PRESSED)*/
        mutable bool m_rose;           /** Determines if the state rose recently (from PRESSED to RELEASED)*/
        mutable bool m_longPress;      /** Long press has been detected */
    };

}


#endif // Button_h
