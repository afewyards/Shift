#ifndef __PLANE
#define __PLANE

namespace Shift {

class Plane {
  private:
    Plane* next;
    virtual void loop() = 0;
    virtual void init() = 0;

    char* _id;

  public:
    static Plane* first;

    Plane() {
      this->next = first;
      first = this;
    }

    void setId(char* id) {
      _id = id;
    }

    static void initPlane(char* id) {
      Plane* pi = first;
      Plane* sel;

      if (!pi) return;

      do {
        if (pi->_id == id) {
          sel = pi;
        }

        if (pi->next == NULL) pi->next = first;
        pi = pi->next;
      } while (pi != first && !sel);

      if (sel) {
         sel->init();
      }
    }

    static void loopPlane(char* id) {
      Plane* pi = first;
      Plane* sel;

      if (!pi) return ;

      do {
        if (pi->_id == id) {
          sel = pi;
        }

        if (pi->next == NULL) pi->next = first;
        pi = pi->next;
      } while (pi != first && !sel);

      if (sel) {
         sel->loop();
      }
    }
};

Plane* Plane::first = NULL;

}

#endif
