#ifndef __SHIFT_POTS
#define __SHIFT_POTS

#include "Input.h"
#include "InputInterface.h"

namespace Shift {

class Potentiometer : Input {
  private:
    int _id = -1;
    int _pin;
    int _max = 0xFFFF;
    uint8_t _steps = 1;

    uint16_t readState() {
      uint16_t data = InputInterface::readDigital(_id, _pin);

      if (_steps == 1)
        return data;

      uint16_t divisor = ((uint16_t)0xFFFF / _steps) ;
      uint16_t output = (data + divisor / 2) / divisor;

      if (output > _max) return _max;
      return output;
    }

    void input() {
      lastState_ = 0;
      pollInput();
    }

    void pollInput() {
      if (_steps == 1) {
        unsigned int state = readState();
        accumulator += ((float)state - accumulator) / (float)ewma_divisor;
        state = (unsigned int)accumulator;

        if (((lastState_ > state && (lastState_ - state > hysteresis)))
            || ((state > lastState_) && (state - lastState_ > hysteresis))
            || ((state > (65535 - hysteresis) && state > lastState_))
            || ((state < hysteresis && state < lastState_))
           ) {
           char buf[6];
           utoa(state, buf, 10);

           if (send(msg_, buf))
             lastState_ = state;
        }
      } else {
        unsigned int state = readState();

        if (state != lastState_) {
          char buf[6];
          utoa(state, buf, 10);

          if (send(msg_, buf))
            lastState_ = state;
        }
      }

    }

    unsigned int pollInterval = 1;
    unsigned int hysteresis = 140;
    unsigned int ewma_divisor = 5;

    const char* msg_;

    unsigned int lastState_;
    float accumulator;
    unsigned char lastPollTime_;

  public:
    Potentiometer(int id, char pin) {
      _id = id;
      _pin = pin;
      lastState_ = readState();
      _steps = 1;
      lastPollTime_ = (unsigned char)millis();
      input();
    }

    Potentiometer(int id, char pin, uint8_t steps) {
      _id = id;
      _pin = pin;
      _steps = steps;
      lastState_ = readState();
      lastPollTime_ = (unsigned char)millis();
      input();
    }

    void SetControl( const char* msg )
    {
      msg_ = msg;
      input();
    }

    void SetMax ( int max ) {
      _max = max;
    }

    void ClearControl() {
      msg_ = NULL;
      _max = 0xFFFF;
    }
};

}

#endif
