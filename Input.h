#ifndef __SHIFT_INPUT
#define __SHIFT_INPUT

#include "DcsBios.h"

namespace Shift {

class Input {
  private:
    virtual void ClearControl() = 0;
    virtual void pollInput() = 0;
    Input* nextInput;

  public:
    static Input* firstInput;
    Input() {
      this->nextInput = firstInput;
      firstInput = this;
    }

    static void clearInputs() {
      Input* pi = firstInput;

      if (!pi) return;

      do {
        pi->ClearControl();
        if (pi->nextInput == NULL) pi->nextInput = firstInput;
        pi = pi->nextInput;
      } while (pi != firstInput);
    }

    static void pollInputs() {
				Input* pi = firstInput;
				if (!pi) return;

				do { // step through circular linked list
					pi->pollInput();
					// make linked list circular if it is not already
					if (pi->nextInput == NULL) pi->nextInput = firstInput;

					pi = pi->nextInput;
				} while (pi != firstInput); // util we get back to the start
			}
};

Input* Input::firstInput = NULL;

}
#endif
