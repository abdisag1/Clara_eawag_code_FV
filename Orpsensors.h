#ifndef MY_ORPSENSORS__H
#define MY_ORPSENSORS_H

#include "Clara.h"

using namespace std;

class Orpsensors{

  private:
   
   int pinA4; 
   int pinA5; 
   
   
  
   

  public:
    Orpsensors(byte pinA4, byte pinA5);
    void init();
    void update_measurement_values();
    void update_conditions_to_change_pumptime ();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
    

};
#endif
