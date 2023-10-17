#ifndef MY_FLOWMETER_H
#define MY_FLOWMETER_H

#include "Clara.h"
using namespace std;

class Flowmeter {

  private:
    byte pin;
    static volatile int flow_count; // Measures flow sensor pulses
    unsigned int flow_frequency;
    double l_hour; // Calculated litres/hour
    double l_min; // Calculated l/min
    double l_sec;
    float calibration_coefficient = 6;     // Used to calculate the flow-rate.

    unsigned long currentTime;
    unsigned long cloopTime;

    static void flow();

  public:
    Flowmeter(byte pin);
    void init();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"

};
#endif
