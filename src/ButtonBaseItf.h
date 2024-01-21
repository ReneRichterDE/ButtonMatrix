

namespace RSys
{
        /**
            @brief Type representing the state of a button
        */
        enum BTN_STATE
        {
            /**
                @brief Button state is not yet initialized
            */
            BTN_STATE_UNINITIALIZED,
            /**
                @brief Button is RELEASED (not pressed)
            */            
            BTN_STATE_RELEASED,
            /**
                @brief Button is PRESSED
            */             
            BTN_STATE_PRESSED
        };


        /**
            @brief Type representing button actions
        */
        enum BTN_ACTION
        {
            BTN_ACTION_NONE,           /** No button action */
            BTN_ACTION_CLICK,          /** Button has been click (notified when button is released) */
            BTN_ACTION_DBL_CLICK,   /** not yet implemented */
            BTN_ACTION_LONG_PRESS      /** Button has been pressed long */
        };


    class ButtonBaseItf
    {
    public:

        /**
            @brief  Updates the button with a new state. 
                    If the state is different to the current state, the change will be notified!
                    (Left the method public to allow usage independent of the ButtonMatrix)
            @return True, if the state has changed or false if the new state is the same as the previous
        */     
        virtual bool updateState(const BTN_STATE newState) = 0;


    };


}