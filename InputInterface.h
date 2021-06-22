#ifndef __INPUTINTERFACE
#define __INPUTINTERFACE

namespace Shift {

class InputInterface {
  private:
    InputInterface* next;

    int _id;
    int _bit;

    virtual int readDigital(int pin) = 0;
    virtual void sync() = 0;
    virtual void init() = 0;

  public:
    static InputInterface* first;

    InputInterface() {
      this->next = first;
      first = this;
    }

    void setId(int id) {
      _id = id;
    }

    static void syncAll() {
      InputInterface* pi = first;

      if (!pi) return;

      do {
        pi->sync();

        if (pi->next == NULL) pi->next = first;
        pi = pi->next;
      } while (pi != first);
    }

    static void reset() {
      InputInterface* pi = first;

      if (!pi) return;

      do {
        pi->init();

        if (pi->next == NULL) pi->next = first;
        pi = pi->next;
      } while (pi != first);
    }

    static int readDigital(int id, int port) {
      InputInterface* pi = first;
      InputInterface* sel;

      if (!pi) return 0;

      do {
        if (pi->_id == id) {
          sel = pi;
        }

        if (pi->next == NULL) pi->next = first;
        pi = pi->next;
      } while (pi != first && !sel);

      if (sel) {
        return sel->readDigital(port);
      }

      return 0;
    }
};

InputInterface* InputInterface::first = NULL;

}

#endif
