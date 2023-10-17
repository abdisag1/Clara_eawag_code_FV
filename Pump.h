#ifndef MY_PUMP_H
#define MY_PUMP_H

#include "Clara.h"
using namespace std;

class Pump {
  
  private:
    bool state;
    byte pin;       
    // Volume of water in milliliter to pass in the flowmeter until a chlorination occurs
    unsigned long pumpInterval; // [ml]
    // Flow rate of the pump in milliliter/hour
    double pumpRate; // [ml/min]  
    // Desired initial concentration of chlorine in the treated water
    double chlorineConcentration = 1.8; // [mg/l]
    // Concentration of the WATA-Solution
    unsigned long wataConcentration = 6; // [g/l] = [mg/ml]

    unsigned long currentTime;
    unsigned long cloopTime;
    double delta_t;
    double delta_V;

    unsigned long pump_time_started; // Time where a pumping-process is started
    unsigned long pump_time_total;
     // Total time of a pumping-process
    
  public:
    Pump(byte pin);
    void init();
    void run(Clara &clara); // Has to be passed as a reference! See p. 255 in "Grundkurs C++"
  
    // state-methods
    bool get_state();
    void set_state(bool state);

    // pump-chlorine-method
    void pump_chlorine(Clara &clara);
    void check_pump();
};
#endif
