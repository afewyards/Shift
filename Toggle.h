#ifndef __SHIFT_SWITCH_THREE
#define __SHIFT_SWITCH_THREE

#include "Input.h"
#include "InputInterface.h"

namespace Shift {

class Toggle : Input {
  private:
    Toggle* nextToggle;

    int _id = -1;
    int _pinA;
    int _pinB;

    const char* _msg = NULL;

    bool _twoway = false;
    bool _reverse = false;
    bool _noDelay = false;
    unsigned int _lastSwitchTime;

    int8_t _lastState;

    int8_t readState() {
      if (InputInterface::readDigital(_id, _pinA) == HIGH) return _twoway ? _reverse ? 0 : 1 : _reverse ? 0 : 2;
      if (InputInterface::readDigital(_id, _pinB) == HIGH) return _twoway ? _reverse ? 1 : 0 : _reverse ? 2 : 0;
      return _twoway ? -1 : 1;
    }

    void pollInput() {
      int8_t state = readState();

      if (state != _lastState && state != -1) {
        unsigned int now = (unsigned int)millis();
        if (((unsigned int)(now - _lastSwitchTime) < 200) && !_noDelay) 
          delay(200);

        _lastSwitchTime = (unsigned int)millis();
        input();
      }
    }

    void input() {
      char * value;
      int8_t state = readState();

      if (state == 0)
        value = "0";
      if (state == 1)
        value = "1";
      if (state == 2)
        value = "2";

      if(send(_msg, value))
        _lastState = state;
    }

  public:
    static Toggle* firstToggle;

    Toggle(int id, int pinA, int pinB) {
      _id = id;
      _pinA = pinA;
      _pinB = pinB;
      _lastState = readState();

      input();

      this->nextToggle = firstToggle;
      firstToggle = this;
    }

    void SetControl( const char* msg )
    {
      _msg = msg;

      input();
    }

    void Set2Way() {
      _twoway = true;
    }

    void SetNoDelay() {
      _noDelay = true;
    }

    void SetReverse() {
      _reverse = true;
    }

    void ClearControl() {
      _twoway = false;
      _reverse = false;
      _noDelay = false;
      _msg = NULL;
    }

    int8_t getState() {
      return _lastState;
    }

    static int readAll() {
      int output = 0;
      int index = 0;
				Toggle* pi = firstToggle;
				if (!pi) return 0;

				do {
          output |= (pi->getState() & 0x2) << (index * 2);
					if (pi->nextToggle == NULL) pi->nextToggle = firstToggle;
					pi = pi->nextToggle;
          index++;
				} while (pi != firstToggle);

        return output;
			}
};

Toggle* Toggle::firstToggle = NULL;
}

#endif
