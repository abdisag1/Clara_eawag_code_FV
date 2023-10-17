#ifndef MY_ECOPHI_H
#define MY_ECOPHI_H

#include "Clara.h"

using namespace std;

class Ecophi {

  private:

       byte pin0;
       byte pin1;
       byte pin2;
      static void timerCallback();


  public:
    Ecophi(byte pin0,byte pin1,byte pin2);
    void init();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
  
};
#endif
