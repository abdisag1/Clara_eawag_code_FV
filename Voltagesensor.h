#ifndef MY_VOLTAGESENSOR_H
#define MY_VOLTAGESENSOR_H

#include "Clara.h"
using namespace std;

class Voltagesensor {

  private:
  int pin; 
const int voltageSensor = A4;

float vOUT = 0.0;
float voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
float high = 13.9;
float low = 13.7;    
float batterypercentage;
   

  public:
    Voltagesensor(byte pin);
    void init();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
    

};
#endif
