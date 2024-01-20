

#include <IOHandlerItf.h>
#include <Button.h>


class SimulatedIOHandler : public RSys::IOHandlerItf
{
public:

    /** IOHandlerItf */
    virtual void pinMode(uint8_t pin, uint8_t mode);
    virtual void digitalWrite(uint8_t pin, uint8_t val);
    virtual int digitalRead(uint8_t pin);

    /** Simulator methods */
    void simButtonState(uint8_t row, uint8_t col, RSys::Button::STATE state);


    static inline SimulatedIOHandler& getInstance(
                                    uint8_t* rowPins, uint8_t* colPins,
                                    uint8_t numRows, uint8_t numCols)
    {            
        return *(new SimulatedIOHandler(rowPins, colPins, numRows, numCols));
    }

private:

    /**
        @brief  Returns the implementation for an SimulatedIOHandler
        @return Reference to the implementation
    */ 
    SimulatedIOHandler(
                    uint8_t* rowPins, uint8_t* colPins,
                    uint8_t numRows, uint8_t numCols);

    virtual ~SimulatedIOHandler();

    bool getLowCol(uint8_t& col) const;
    bool getRowFromPin(uint8_t pin, uint8_t& row) const;
    bool getColFromPin(uint8_t pin, uint8_t& col) const;

    

    const uint8_t*  m_rowPins;      /** Array of row pins */
    const uint8_t*  m_colPins;      /** Array of column pins */
    const uint8_t   m_numRows;      /** Number of rows in the matrix */
    const uint8_t   m_numCols;      /** Number of columns in the matrix */

    int* m_pIoStates;
    RSys::Button::STATE* m_pButtonStates;
};