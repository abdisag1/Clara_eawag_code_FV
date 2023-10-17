#ifndef MY_Wata_H
#define MY_Wata_H

#include "Clara.h"
using namespace std;

class Wata {

  private:
   int pin10;
   int pin11; 
   int v_state;
   int settle_state;
   int valvecome; 
   int wa_state;
   int watacome;

  public:
    Wata(byte pin10,byte pin11);
    void init();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
    

};
#endif
