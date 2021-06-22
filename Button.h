#ifndef __SHIFT_BUTTON
#define __SHIFT_BUTTON

#include "Input.h"
#include "InputInterface.h"

namespace Shift {

class Button : Input {
  private:
    Button* nextButton;

    int _id = -1;
    int _pin;
    int _lastState;
    int _current = 0;
    bool _latching;

    const char* _msg = NULL;

    bool readState() {
      return InputInterface::readDigital(_id, _pin);
    }

    void pollInput() {
      bool state = readState();

      if (state != _lastState) {
        input();
      }
    }

    void input() {
      bool state = readState();

      if (_latching) {
        _lastState = state;
        if (state) {
          _current = _current == 1 ?  0 : 1 ;
          send(_msg, _current ? "1" : "0");
        }
      } else {
        if (send(_msg, state ? "1" : "0"))
          _lastState = state;
      }
    }

  public:
    static Button* firstButton;

    Button(int id, int pin) {
      _id = id;
      _pin = pin;
      _latching = false;
      _lastState = readState();

      this->nextButton = firstButton;
      firstButton = this;
    }

    void SetLatching( bool latching ) {
      _latching = latching;
    }

    void SetControl( const char* msg )
    {
      _msg = msg;

      input();
    }

    void ClearControl() {
      _msg = NULL;
      _latching = false;
      _current = 0;
    }

    int getState() {
      return _lastState;
    }

    static int readAll() {
			Button* pi = firstButton;
      int output = 0;
      int index = 0;

		  if (!pi) return 0;

			do { // step through circular linked list
         output |= (pi->getState() & 0x1) << index;
				// make linked list circular if it is not already
				if (pi->nextButton == NULL) pi->nextButton = firstButton;
				pi = pi->nextButton;
        index++;
			} while (pi != firstButton); // util we get back to the start
 
      return output;
		}
};

Button* Button::firstButton = NULL;

}

#endif
