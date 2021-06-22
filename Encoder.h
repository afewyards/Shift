#ifndef __SHIFT_ENCODER
#define __SHIFT_ENCODER

#include "Input.h"
#include "InputInterface.h"

namespace Shift {

class Encoder : Input {
  private:
    const char* msg_ = NULL;
    const char* msgPressed_ = NULL;
    const char* msgPressedLeft_ = NULL;
    const char* msgPressedRight_ = NULL;
    const char* decArg_;
    const char* incArg_;
    const char* decPressedArg_;
    const char* incPressedArg_;
    int _id = -1;
    int _pinPressed = -1;

    int16_t lastCount = 0;

    signed char delta_;
    char stepsPerDetent_;
    unsigned long lastChange;

    int _timer;

    TIM_HandleTypeDef * getTimer() {
      switch (_timer) {
        case 1:
          return &htim1;
        case 3:
          return &htim3;
        case 4:
          return &htim4;
      }
    }

    int8_t getCount() {
      int16_t c = __HAL_TIM_GET_COUNTER(getTimer());
      return (int16_t)c ;
    }

    void resetState() {
      __HAL_TIM_SET_COUNTER(getTimer(), 0);
    }

    void setState(uint8_t value) {
      __HAL_TIM_SET_COUNTER(getTimer(), value);
    }

    char readPressedPin() {
      return InputInterface::readDigital(_id, _pinPressed) == HIGH;
    }

    void pollInput() {
      int c = getCount();
      int r = __HAL_TIM_GET_COUNTER(getTimer());
      const char* msg = msg_;
      bool isPressed = _pinPressed > -1 && readPressedPin();

      if (isPressed && msgPressed_ != NULL) {
        msg = msgPressed_;
      }

      if (c != lastCount) {
        lastCount = c;

        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(getTimer())) {
          if (msgPressedLeft_ && isPressed) {
            if (send(msgPressedRight_, "1")) {
              delay(20);
              send(msgPressedRight_, "0");
            }
          } else {
            send(msg, incArg_);
          }
        } else {
          if (msgPressedLeft_ && isPressed) {
            if (send(msgPressedLeft_, "1")) {
              delay(20);
              send(msgPressedLeft_, "0");
            }
          } else {
            send(msg, decArg_);
          }
        }
      }
    }

  public:
    Encoder(const char* decArg, const char* incArg, int timer) {
      decArg_ = decArg;
      incArg_ = incArg;
      _timer = timer;
    }

    Encoder(const char* decArg, const char* incArg, int timer, int id, int pinPressed) {
      decArg_ = decArg;
      incArg_ = incArg;
      _timer = timer;
      _id = id;
      _pinPressed = pinPressed;
    }

    //////////////////////////////////////////
    //  Inserted Code
    //
    void SetControl( const char* msg )
    {
      msg_ = msg;
      msgPressed_ = NULL;
      msgPressedLeft_ = NULL;
      msgPressedRight_ = NULL;
    }
    void SetControl( const char* msg, const char* msgPressed )
    {
      msg_ = msg;
      msgPressed_ = msgPressed;
      msgPressedLeft_ = NULL;
      msgPressedRight_ = NULL;
    }
    void SetControl( const char* msg, const char* msgPressedLeft , const char* msgPressedRight )
    {
      msg_ = msg;
      msgPressed_ = NULL;
      msgPressedLeft_ = msgPressedLeft;
      msgPressedRight_ = msgPressedRight;
    }
    void ClearControl( )
    {
      msg_ = NULL;
      msgPressed_ = NULL;
      msgPressedLeft_ = NULL;
      msgPressedRight_ = NULL;
    }
};

}

#endif
